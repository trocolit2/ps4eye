/*
 * PS4eye.cpp
 *
 *  Created on: Sep 11, 2015
 *      Author: tiagotrocoli
 */

#include <DeviceUtils.h>
#include <PS4eye.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

#define PS4EYE_DEVICE_IDVENDOR "05a9"
#define PS4EYE_DEVICE_IDPRODUCT "058a"
#define PS4EYE_DEVICE_SYSNAME "video"

#define PS4EYE_RESOLUTION_HD_WIDTH 3448
#define PS4EYE_RESOLUTION_HD_HEIGHT 808

#define PS4EYE_RESOLUTION_VGA_WIDTH 1748
#define PS4EYE_RESOLUTION_VGA_HEIGHT 408

#define PS4EYE_RESOLUTION_VGA_LOW_WIDTH 640
#define PS4EYE_RESOLUTION_VGA_LOW_HEIGHT 200

PS4eye::PS4eye(std::string device, resolutionEnum resolution, float frameRate) {
    if (resolution == PS4EYE_RESOLUTION_320x200) {
        this->videoDevice = new V4LTools(device.c_str(), PS4EYE_RESOLUTION_VGA_LOW_HEIGHT, PS4EYE_RESOLUTION_VGA_LOW_WIDTH, frameRate);
        this->roi = cv::Rect(48, 0, 320 * 2, 200);
        this->rawFrame = cv::Mat(PS4EYE_RESOLUTION_VGA_LOW_HEIGHT, PS4EYE_RESOLUTION_VGA_LOW_WIDTH, CV_8UC2);
        this->frame = cv::Mat(roi.height, roi.width, CV_8UC3);

    } else if (resolution == PS4EYE_RESOLUTION_1280x800) {
        this->videoDevice = new V4LTools(device.c_str(), PS4EYE_RESOLUTION_HD_HEIGHT, PS4EYE_RESOLUTION_HD_WIDTH, frameRate);
        this->roi = cv::Rect(48, 0, 1280 * 2, 800);
        this->rawFrame = cv::Mat(PS4EYE_RESOLUTION_HD_HEIGHT, PS4EYE_RESOLUTION_HD_WIDTH, CV_8UC2);
        this->frame = cv::Mat(roi.height, roi.width, CV_8UC3);

    } else {
        this->videoDevice = new V4LTools(device.c_str(), PS4EYE_RESOLUTION_VGA_HEIGHT, PS4EYE_RESOLUTION_VGA_WIDTH, frameRate);
        this->roi = cv::Rect(48, 0, 1280, 400);
        this->rawFrame = cv::Mat(PS4EYE_RESOLUTION_VGA_HEIGHT, PS4EYE_RESOLUTION_VGA_WIDTH, CV_8UC2);
        this->frame = cv::Mat(roi.height, roi.width, CV_8UC3);
    }
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
    this->rawFrame.data = videoDevice->graFrame();
    cv::cvtColor(this->rawFrame(roi), this->frame, CV_YUV2BGR_YUYV);
    return frame;
}

PS4eye::resolutionEnum PS4eye::getResolution() {
    uint height, width;
    float fps = 0;
    this->videoDevice->getCameraParameters(height, width, fps);

    resolutionEnum resolution;
    switch (height) {
    case PS4EYE_RESOLUTION_VGA_HEIGHT:
        resolution = PS4EYE_RESOLUTION_640x400;
        break;
    case PS4EYE_RESOLUTION_HD_HEIGHT:
        resolution = PS4EYE_RESOLUTION_1280x800;
        break;
    case PS4EYE_RESOLUTION_VGA_LOW_HEIGHT:
        resolution = PS4EYE_RESOLUTION_320x200;
        break;
    }
    return resolution;
}
PS4eye::frameRateEnum PS4eye::getFrameRate() {

    uint height, width;
    float fps = 0;
    this->videoDevice->getCameraParameters(height, width, fps);
    return (int) fps;
}

PS4eye::~PS4eye() {
    delete (videoDevice);
}

bool PS4eye::isActive() {
    return this->videoDevice->isActive();
}
