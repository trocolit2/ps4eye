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

    std::vector<std::string> devicesPaths;
    PS4eye::ps4eyeDevices(&devicesPaths);

    PS4eye ps4eyeDevice(devicesPaths[0], PS4eye::PS4EYE_RESOLUTION_1280x800, PS4eye::PS4EYE_FPS_30);

    cv::Mat frame;
    while (ps4eyeDevice.isActive()) {
        cv::imshow("OUT IMAGE", ps4eyeDevice.grabFrame());
        cv::waitKey(1);
    }

}

//BOOST_AUTO_TEST_CASE(checkPS4Eye) {
//
//}
