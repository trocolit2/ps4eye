/*
 * PS4eye.h
 *
 *  Created on: Sep 11, 2015
 *      Author: tiagotrocoli
 */

#ifndef SOURCE_DIRECTORY__SRC_PS4EYE_H_
#define SOURCE_DIRECTORY__SRC_PS4EYE_H_

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class PS4eye {
public:

    enum resolutionEnum {
        PS4EYE_RESOLUTION_160x100,
        PS4EYE_RESOLUTION_320x200,
        PS4EYE_RESOLUTION_640x400 = 1748,
        PS4EYE_RESOLUTION_1280x800 = 3448
    };

    enum frameRateEnum {
        PS4EYE_FPS_15,
        PS4EYE_FPS_30,
        PS4EYE_FPS_60,
        PS4EYE_FPS_120,
        PS4EYE_FPS_240
    };

    PS4eye(unsigned int deviceNum, resolutionEnum resolution = PS4EYE_RESOLUTION_640x400, frameRateEnum frameRate = PS4EYE_FPS_30);

    cv::Mat grabFrame();

    resolutionEnum getResolution();
    frameRateEnum getFrameRate();

    void setResolution(resolutionEnum);
    void setFrameRate(frameRateEnum);

    static std::vector<unsigned int> ps4eyeDevices(std::vector<std::string> *path = 0);

private:
    cv::VideoCapture videoCapture;
};

#endif /* SOURCE_DIRECTORY__SRC_PS4EYE_H_ */
