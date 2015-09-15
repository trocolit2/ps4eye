/*
 * PS4eye.cpp
 *
 *  Created on: Sep 11, 2015
 *      Author: tiagotrocoli
 */

#include <DeviceUtils.h>
#include <PS4eye.h>
#include <cstring>
#include <string>
#include <iostream>

#define PS4EYE_DEVICE_IDVENDOR "05a9"
#define PS4EYE_DEVICE_IDPRODUCT "058a"
#define PS4EYE_DEVICE_SYSNAME "video"

PS4eye::PS4eye(unsigned int deviceNum, resolutionEnum resolution = PS4EYE_RESOLUTION_640x400, frameRateEnum frameRate = PS4EYE_FPS_30) {
    videoCapture.set(CV_CAP_PROP_FRAME_WIDTH, (double) PS4EYE_RESOLUTION_640x400);
    videoCapture = cv::VideoCapture(deviceNum);
}

std::vector<unsigned int> PS4eye::ps4eyeDevices(std::vector<std::string> *path) {

    DeviceUtils utils;
    std::vector<deviceAttributes> devicesFromID = utils.getDeviceAtributesFromIdVendorAndIdProductor(std::string(PS4EYE_DEVICE_IDVENDOR), std::string(PS4EYE_DEVICE_IDPRODUCT));
    std::vector<deviceAttributes> devicesFromSysname;
    unsigned int video = 0;
    while (true) {
        std::vector<deviceAttributes> devices = utils.getDeviceAtributesFromSysname(std::string(PS4EYE_DEVICE_SYSNAME) + std::to_string(video));
        if (devices.size() != 0)
            devicesFromSysname.insert(devicesFromSysname.end(), devices.begin(), devices.end());
        else
            break;
        ++video;
    }
    std::vector<unsigned int> deviceNum;
    std::vector<std::string> devicePath;
    for (unsigned int i = 0; i < devicesFromID.size(); ++i)
        for (unsigned int j = 0; j < devicesFromSysname.size(); ++j)
            if (strstr(devicesFromSysname[j].devpath.c_str(), devicesFromID[i].devpath.c_str()) != NULL) {
                deviceNum.push_back(j);
                devicePath.push_back(devicesFromSysname[j].devnode);
            }
    if (path != 0)
        path->insert(path->end(), devicePath.begin(), devicePath.end());
    return deviceNum;
}

cv::Mat PS4eye::grabFrame() {
    cv::Mat frame;
    videoCapture >> frame;
    return frame;
}

PS4eye::resolutionEnum PS4eye::getResolution() {
    double out = videoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
    std::cout << " LARGURA " << out << std::endl;
    return PS4EYE_RESOLUTION_640x400;
}
PS4eye::frameRateEnum PS4eye::getFrameRate() {

    double out = videoCapture.get(CV_CAP_PROP_FPS);
    std::cout << " FPS " << out << std::endl;
    return PS4EYE_FPS_30;
}

void setResolution(PS4eye::resolutionEnum) {

}
void setFrameRate(PS4eye::frameRateEnum) {
}

