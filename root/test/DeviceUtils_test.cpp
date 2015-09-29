/*
 *
 *  Created on: Sep 14, 2015
 *      Author: tiagotrocoli
 */

#include <DeviceUtils.h>
#include <string>

#define BOOST_TEST_DYN_LINK
//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "DeviceUtils_test"
//VERY IMPORTANT - include this last

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>

DeviceUtils utils;

BOOST_AUTO_TEST_CASE(checkSYSNAMEDeviceSearch_testCase) {

    std::vector<deviceAttributesSearch> attributes = { SYSNAME };
    std::vector<std::string> values = { "video0" };
    std::vector<deviceAttributes> devices = utils.getDevicesFrom(attributes, values);
    BOOST_CHECK_EQUAL(devices.size(), 1);
}

BOOST_AUTO_TEST_CASE(checkSUBSYSTEMDeviceSearch_testCase) {

    std::vector<deviceAttributesSearch> attributes = { SUBSYSTEM };
    std::vector<std::string> values = { "hidraw" };
    std::vector<deviceAttributes> devices = utils.getDevicesFrom(attributes, values);

    BOOST_CHECK_EQUAL(devices.size() > 0, true);
}

BOOST_AUTO_TEST_CASE(checkIDVENDORandIDPRODUCTDeviceSearch_testCase) {

    std::vector<deviceAttributesSearch> attributes = { IDVENDOR, IDPRODUCT };
    std::vector<std::string> values = { "05a9", "058a" };
    std::vector<deviceAttributes> devices = utils.getDevicesFrom(attributes, values);
    BOOST_CHECK_EQUAL(devices.size(), 1);
}
