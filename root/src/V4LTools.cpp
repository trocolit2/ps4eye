/*
 * V4LTools.cpp
 *
 *  Created on: Sep 15, 2015
 *      Author: tiagotrocoli
 */

#include <V4LTools.h>
#include <iostream>

#define VIDEO_FORMAT V4L2_PIX_FMT_YUYV

V4LTools::V4LTools(std::string devicePath, unsigned int height, int width, float fps) {

    this->videoIn = (struct vdIn *) calloc(1, sizeof(struct vdIn));

    if (init_videoIn(this->videoIn, (char*) devicePath.c_str(), width, height, fps, VIDEO_FORMAT, 1, 0) < 0)
        this->videoIn = 0;
}

V4LTools::~V4LTools() {

    close_v4l2(this->videoIn);
    free(this->videoIn);
    sleep(5);

}

unsigned char* V4LTools::graFrame() {
    if (uvcGrab(this->videoIn) < 0) {
        std::cout << "Error grabbing";
        return 0;
    }
    return this->videoIn->framebuffer;
}

bool V4LTools::isActive() {
    return this->videoIn->signalquit;
}

void V4LTools::resetCameraParameters(unsigned int height, unsigned int width, float fps) {
    std::string currentDevice(this->videoIn->videodevice);

    close_v4l2(this->videoIn);
    if (init_videoIn(this->videoIn, (char*) currentDevice.c_str(), width, height, fps, VIDEO_FORMAT, 1, 0) < 0)
        this->videoIn = 0;

}

void V4LTools::getCameraParameters(unsigned int &height, unsigned int &width, float &fps) {
    height = this->videoIn->height;
    width = this->videoIn->width;
    fps = this->videoIn->fps;
}

