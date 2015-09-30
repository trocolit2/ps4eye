/*
 * PS4eye_test.cpp
 *
 *  Created on: Sep 11, 2015
 *      Author: tiagotrocoli
 */

#include <PS4eye.h>
#include <iostream>
#include <string>
#include <sys/timeb.h>

#define BOOST_TEST_DYN_LINK
//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "PS4eye_test"
//VERY IMPORTANT - include this last

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>

static struct timeb start, end;
static uint limitFrames = 100;

static std::string devicePath;
struct defineDevice {
    defineDevice() {
        std::vector<std::string> devicesPath;
        PS4eye::ps4eyeDevices(&devicesPath);
        devicePath = devicesPath[0];
    }
};

BOOST_GLOBAL_FIXTURE(defineDevice);

BOOST_AUTO_TEST_CASE(checkPS4eyeLoadDeviceWithVGAAnd30Fps) {

    PS4eye *ps4eyeDevice = new PS4eye(devicePath, PS4eye::PS4EYE_RESOLUTION_1280x800, PS4eye::PS4EYE_FPS_30);

    uint countFrames = 0;
    double avgFps = 0;

    cv::Mat frame;
    for (uint i = 0; i < limitFrames; ++i) {
        ftime(&start);
        frame = ps4eyeDevice->grabFrame();
        ftime(&end);
        avgFps += 1.0 / ((end.time - start.time) + (end.millitm - start.millitm) * 0.001);
        ++countFrames;
    }

    delete (ps4eyeDevice);

    BOOST_CHECK_EQUAL(frame.cols, 1280 * 2);
    BOOST_CHECK_EQUAL(frame.rows, 800);
    BOOST_CHECK_GE(ceil(avgFps / countFrames), 30.0);
}

//BOOST_AUTO_TEST_CASE(checkPS4eyeLoadDeviceWithVGAAnd60Fps) {
//
//    PS4eye ps4eyeDevice(devicePath, PS4eye::PS4EYE_RESOLUTION_640x400, PS4eye::PS4EYE_FPS_60);
//
//    uint countFrames = 0;
//    double avgFps = 0;
//
//    cv::Mat frame;
//    for (uint i = 0; i < limitFrames; ++i) {
//        ftime(&start);
//
//        frame = ps4eyeDevice.grabFrame();
//
//        ftime(&end);
//        avgFps += 1.0 / ((end.time - start.time) + (end.millitm - start.millitm) * 0.001);
//        ++countFrames;
////        std::cout << "\r FPS =  " << avgFps / countFrames << std::endl;
//    }
//
//    BOOST_CHECK_EQUAL(frame.cols, 640 * 2);
//    BOOST_CHECK_EQUAL(frame.rows, 400);
//    BOOST_CHECK_GE(ceil(avgFps / countFrames), 60.0);
//}
//
//BOOST_AUTO_TEST_CASE(checkPS4eyeLoadDeviceWithVGAAnd120Fps) {
//
//    sleep(5);
//    PS4eye ps4eyeDevice(devicePath, PS4eye::PS4EYE_RESOLUTION_640x400, PS4eye::PS4EYE_FPS_120);
//
//    uint countFrames = 0;
//    double avgFps = 0;
//
//    cv::Mat frame;
//    for (uint i = 0; i < limitFrames; ++i) {
//        ftime(&start);
//
//        frame = ps4eyeDevice.grabFrame();
//
//        ftime(&end);
//        avgFps += 1.0 / ((end.time - start.time) + (end.millitm - start.millitm) * 0.001);
//        ++countFrames;
////        std::cout << "\r FPS =  " << avgFps / countFrames << std::endl;
//    }
//
//    BOOST_CHECK_EQUAL(frame.cols, 640 * 2);
//    BOOST_CHECK_EQUAL(frame.rows, 400);
//    BOOST_CHECK_GE(ceil(avgFps / countFrames), 120);
//}
