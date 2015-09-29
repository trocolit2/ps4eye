/*
 * V4LTools.h
 *
 *  Created on: Sep 15, 2015
 *      Author: tiagotrocoli
 */

#ifndef SRC_V4LTOOLS_H_
#define SRC_V4LTOOLS_H_

#include <string>

extern "C" {
#include "v4ltool/v4l2uvc.h"
}

class V4LTools {
public:
    V4LTools(std::string devicePath, unsigned int height = 480, int width = 640, float fps = 30.0);
    ~V4LTools();

    unsigned char* graFrame();

    bool isActive();
    void resetCameraParameters(unsigned int height, unsigned int width, float fps);
    void getCameraParameters(unsigned int &height, unsigned int &width, float &fps);
    void release();

private:
    struct vdIn *videoIn;

};

#endif /* SRC_V4LTOOLS_H_ */
