/*
 * V4LTools_test.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: tiagotrocoli
 */

#include <V4LTools.h>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "V4LTools_test"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

uint8_t *buffer;
struct v4l2_format fmt;

static int xioctl(int fd, int request, void *arg) {
    int r;

    do
        r = ioctl(fd, request, arg);
    while (-1 == r && EINTR == errno);

    return r;
}

int print_caps(int fd) {
    struct v4l2_capability caps = { };
    if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &caps)) {
        perror("Querying Capabilities");
        return 1;
    }

    printf("Driver Caps:\n"
            "  Driver: \"%s\"\n"
            "  Card: \"%s\"\n"
            "  Bus: \"%s\"\n"
            "  Version: %d.%d\n"
            "  Capabilities: %08x\n", caps.driver, caps.card, caps.bus_info, (caps.version >> 16) && 0xff, (caps.version >> 24) && 0xff, caps.capabilities);

    struct v4l2_cropcap cropcap = { 0 };
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
        perror("Querying Cropping Capabilities");
        return 1;
    }

    printf("Camera Cropping:\n"
            "  Bounds: %dx%d+%d+%d\n"
            "  Default: %dx%d+%d+%d\n"
            "  Aspect: %d/%d\n", cropcap.bounds.width, cropcap.bounds.height, cropcap.bounds.left, cropcap.bounds.top, cropcap.defrect.width, cropcap.defrect.height, cropcap.defrect.left,
            cropcap.defrect.top, cropcap.pixelaspect.numerator, cropcap.pixelaspect.denominator);

    int support_grbg10 = 0;

    struct v4l2_fmtdesc fmtdesc = { 0 };
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    char fourcc[5] = { 0 };
    char c, e;
    printf("  FMT : CE Desc\n--------------------\n");
    while (0 == xioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc)) {
        strncpy(fourcc, (char *) &fmtdesc.pixelformat, 4);
        if (fmtdesc.pixelformat == V4L2_PIX_FMT_SGRBG10)
            support_grbg10 = 1;
        c = fmtdesc.flags & 1 ? 'C' : ' ';
        e = fmtdesc.flags & 2 ? 'E' : ' ';
        printf("  %s: %c%c %s\n", fourcc, c, e, fmtdesc.description);
        fmtdesc.index++;
    }
    /*
     if (!support_grbg10)
     {
     printf("Doesn't support GRBG10.\n");
     return 1;
     }*/

    fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 2000;
    fmt.fmt.pix.height = 1000;
    fmt.fmt.vbi.sampling_rate = 30;
    //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24;
    //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_GREY;
//    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt)) {
        perror("Setting Pixel Format");
        return 1;
    }

    strncpy(fourcc, (char *) &fmt.fmt.pix.pixelformat, 4);
    printf("Selected Camera Mode:\n"
            "  Width: %d\n"
            "  Height: %d\n"
            "  PixFmt: %s\n"
            "  Field: %d\n"
            "  FPS: %d\n", fmt.fmt.pix.width, fmt.fmt.pix.height, fourcc, fmt.fmt.pix.field, fmt.fmt.vbi.sampling_rate);
    return 0;
}

int init_mmap(int fd) {
    struct v4l2_requestbuffers req = { 0 };
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) {
        perror("Requesting Buffer");
        return 1;
    }

    struct v4l2_buffer buf = { 0 };
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf)) {
        perror("Querying Buffer");
        return 1;
    }

    buffer = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);
    printf("Length: %d\nAddress: %p\n", buf.length, buffer);
    printf("Image Length: %d\n", buf.bytesused);

    return 0;
}

int capture_image(int fd, bool *camAtice) {
    struct v4l2_buffer buf = { 0 };
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    if (-1 == xioctl(fd, VIDIOC_QBUF, &buf)) {
        perror("Query Buffer");
        return 1;
    }

    fd_set fds;
    if (!(*camAtice)) {
        if (-1 == xioctl(fd, VIDIOC_STREAMON, &buf.type)) {
            perror("Start Capture");
            return 1;
        }
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        (*camAtice) = true;
    }
    struct timeval tv = { 0 };
    tv.tv_sec = 2;
    int r = select(fd + 1, &fds, NULL, NULL, &tv);
    if (-1 == r) {
        perror("Waiting for Frame");
        return 1;
    }

    if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) {
        perror("Retrieving Frame");
        return 1;
    }
//    printf("saving image\n");

//    IplImage* frame;
//    CvMat cvmat = cvMat(fmt.fmt.pix.height, fmt.fmt.pix.width, CV_8UC4, (void*) buffer);
//    frame = cvDecodeImage(&cvmat, 1);
//    cvNamedWindow("window", CV_WINDOW_AUTOSIZE);
//    cvShowImage("window", frame);
//    cvWaitKey(0);
//    cvSaveImage("image.jpg", frame, 0);

    uint height = fmt.fmt.pix.height;
    uint width = fmt.fmt.pix.width;
    uint fps_set = fmt.fmt.vbi.sampling_rate;
//    uint fps_set = fmt.fmt.vbi.sampling_rate;
//   cv::Mat myuv(height + height / 2, width, CV_8UC1, (void*) buffer);
    cv::Mat myuv(height, width, CV_8UC2, (void*) buffer);
    cv::Mat mrgb(height, width, CV_8UC3);
    cv::cvtColor(myuv, mrgb, CV_YUV2BGR_YUYV);
    cv::imshow("OUT IMAGE", mrgb);
    if (cv::waitKey(1) == 27)
        return 1;

    return 0;
}

BOOST_AUTO_TEST_CASE(checkPS4Eye) {
//int main() {
    int fd;

    fd = open("/dev/video1", O_RDWR);
    if (fd == -1) {
        perror("Opening video device");
//        return 1;
    }
    if (print_caps(fd))
        perror("ERROR 1");
//        return 1;

    if (init_mmap(fd))
        perror("ERROR 2");
//        return 1;
    int i;
    bool camAtice = false;
    while (true) {
//    for (i = 0; i < 5; i++) {
        if (capture_image(fd, &camAtice))
            perror("ERROR 3");
//            return 1;
    }
    close(fd);
}
