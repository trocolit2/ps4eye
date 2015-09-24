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

BOOST_AUTO_TEST_CASE(checkPS4eyeDeviceSearch) {
//
//    std::vector<std::string> devicePath;
//    std::vector<uint> deviceNum = PS4eye::ps4eyeDevices(&devicePath);
//
//    std::cout << "TESTE METHOD SEARCH PS4eye DEVICES" << std::endl;
//    for (int i = 0; i < deviceNum.size(); ++i) {
//        std::cout << "DEVICE NUM: " << deviceNum[i] << " PATH: " << devicePath[i] << std::endl;
//        BOOST_CHECK_EQUAL("/dev/video1", devicePath[i]);
//        BOOST_CHECK_EQUAL(1, deviceNum[i]);
//    }

}

BOOST_AUTO_TEST_CASE(checkPS4Eye) {

//    struct timeb start, end;
//
//    PS4eye ps4eye(PS4eye::ps4eyeDevices()[0]);
//
//    while (true) {
//        ftime(&start);
//
//        cv::imshow("PS4Eye", ps4eye.grabFrame());
//        ps4eye.getFrameRate();
//        ps4eye.getResolution();
//        cv::waitKey(1);
//
//        ftime(&end);
//        double sec = ((end.time - start.time) + (end.millitm - start.millitm) / 1000.0);
//        uint fps = 1.0 / sec * 1.0;
//
//        std::cout << " PS4EYE FPS =  " << fps;
//        std::cout << " SECONDS =  " << sec;
//        std::cout << std::endl;
//
//    }

}
