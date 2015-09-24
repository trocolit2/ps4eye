/*
 * test.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: tiagotrocoli
 */

// from luvc
extern "C" {
#include "v4l2uvc.h"
}

#include <omp.h>
#include <sys/timeb.h>
#include <linux/videodev2.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#define RESOLUTION_HD_WIDTH 3448
#define RESOLUTION_HD_HEIGHT 808

#define RESOLUTION_VGA_WIDTH 1748
#define RESOLUTION_VGA_HEIGHT 408

struct vdIn *videoIn;

static cv::Mat imageOut;
static cv::Mat image;

static cv::Rect frame_HD(48, 0, 1280 * 2, 800);
static cv::Rect frame_VGA(48, 0, 1280, 400);

int main(int argc, char **argv) {

    int width = RESOLUTION_HD_WIDTH;
    int height = RESOLUTION_HD_HEIGHT;
    float fps = 120;
    int format = V4L2_PIX_FMT_YUYV;
    int grabmethod = 1;
    char *avifilename = NULL;
    const char* videodevice = "/dev/video1";

    videoIn = (struct vdIn *) calloc(1, sizeof(struct vdIn));

    check_videoIn(videoIn, (char*) videodevice);
    if (init_videoIn(videoIn, (char*) videodevice, width, height, fps, format, grabmethod, avifilename) < 0)
        exit(1);

    image = cv::Mat(videoIn->height, videoIn->width, CV_8UC2);
    imageOut = cv::Mat(frame_HD.height, frame_HD.width, CV_8UC3);

    struct timeb start, end;

    std::string nameWindow = "outImage";
    cv::namedWindow(nameWindow, CV_WINDOW_AUTOSIZE);

    while (videoIn->signalquit) {
        ftime(&start);

        if (uvcGrab(videoIn) < 0) {
            std::cout << "Error grabbing";
            break;
        }

        image.data = videoIn->framebuffer;
        cv::cvtColor(image(frame_HD), imageOut, CV_YUV2BGR_YUYV);
        cv::imshow(nameWindow, imageOut);
        cv::waitKey(1);

        ftime(&end);
        double sec = ((end.time - start.time) + (end.millitm - start.millitm) * 0.001);
        uint fps = 1.0 / sec;

        std::cout << "\r FPS =  " << fps << std::endl;
    }
    std::cout << "OK, ATE O FIM" << std::endl;

    return 0;
}

