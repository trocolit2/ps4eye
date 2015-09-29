/*
 * PS4eye.h
 *
 *  Created on: Sep 11, 2015
 *      Author: tiagotrocoli
 */

#ifndef SOURCE_DIRECTORY__SRC_PS4EYE_H_
#define SOURCE_DIRECTORY__SRC_PS4EYE_H_

#include <opencv2/core/core.hpp>
#include <V4LTools.h>
#include <string>
#include <vector>

class PS4eye {
public:

    enum resolutionEnum {
        PS4EYE_RESOLUTION_320x200,
        PS4EYE_RESOLUTION_640x400,
        PS4EYE_RESOLUTION_1280x800
    };

    enum frameRateEnum {
        PS4EYE_FPS_30 = 30,
        PS4EYE_FPS_60 = 60,
        PS4EYE_FPS_120 = 120
    };

    PS4eye(std::string device, resolutionEnum resolution = PS4EYE_RESOLUTION_640x400, float frameRate = PS4EYE_FPS_30);
    ~PS4eye();

    bool isActive();
    cv::Mat grabFrame();
    resolutionEnum getResolution();
    frameRateEnum getFrameRate();
    void release();

    static std::vector<unsigned int> ps4eyeDevices(std::vector<std::string> *path = 0);

private:
    V4LTools* videoDevice;
    cv::Rect roi;
    cv::Mat rawFrame;
    cv::Mat frame;
};

#endif /* SOURCE_DIRECTORY__SRC_PS4EYE_H_ */
