/*
 * V4LTools_test.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: tiagotrocoli
 */

#include <V4LTools.h>
#include <PS4eye.h>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "V4LTools_test"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>

#include <iostream>
#include <exception>
#include <sys/timeb.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RESOLUTION_VGA_WIDTH 1748
#define RESOLUTION_VGA_HEIGHT 408
static cv::Rect frame_VGA(48, 0, 1280, 400);

#define RESOLUTION_HD_WIDTH 3448
#define RESOLUTION_HD_HEIGHT 808
static cv::Rect frame_HD(48, 0, 1280 * 2, 800);

static std::string devicePath;

struct defineDevice {
    defineDevice() {
        std::vector<std::string> devicesPath;
        PS4eye::ps4eyeDevices(&devicesPath);
        devicePath = devicesPath[0];
    }
};

BOOST_GLOBAL_FIXTURE(defineDevice);

BOOST_AUTO_TEST_CASE(checkV4lToolGoodInit) {

//    cv::Mat rawImage(RESOLUTION_VGA_HEIGHT, RESOLUTION_VGA_WIDTH, CV_8UC2);
//    cv::Mat stereoImage(frame_VGA.height, frame_VGA.width, CV_8UC3);
//        cv::cvtColor(rawImage(frame_VGA), stereoImage, CV_YUV2BGR_YUYV);


    V4LTools videoDevice(devicePath, RESOLUTION_VGA_HEIGHT, RESOLUTION_VGA_WIDTH);

    bool hasData = false;

    unsigned char * buf = 0;

    for (int var = 0; var < 10; ++var) {
        buf = videoDevice.graFrame();
        hasData = (buf != 0);
    }

    BOOST_CHECK_EQUAL(true, hasData);
}

//BOOST_AUTO_TEST_CASE(checkV4lToolInitAndChangeCameraParameters) {
//
////    cv::Mat rawImage(RESOLUTION_VGA_HEIGHT, RESOLUTION_VGA_WIDTH, CV_8UC2);
////    cv::Mat stereoImage(frame_VGA.height, frame_VGA.width, CV_8UC3);
////        cv::cvtColor(rawImage(frame_VGA), stereoImage, CV_YUV2BGR_YUYV);
//
//    V4LTools videoDevice(devicePath, RESOLUTION_HD_HEIGHT, RESOLUTION_HD_WIDTH);
//    unsigned char *bufHD = videoDevice.graFrame();
//
////    videoDevice.resetCameraParameters(RESOLUTION_VGA_HEIGHT, RESOLUTION_VGA_HEIGHT, 120);
////    unsigned char *bufVGA = videoDevice.graFrame();
////
////    unsigned int width = 0, height = 0;
////    float fps = 0;
////    videoDevice.getCameraParameters(height, width, fps);
//
////    BOOST_CHECK_EQUAL(true, hasData);
//}
