/*
 * simple_test.cpp
 *
 *  Created on: Sep 30, 2015
 *      Author: tiagotrocoli
 */

#include <PS4eye.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <sys/timeb.h>

int main(int argc, char **argv) {

    std::vector<std::string> devicesPath;
    PS4eye::ps4eyeDevices(&devicesPath);

    if (devicesPath.empty()) {
        std::cout << "NO PS4EYE DEVICE FOUND" << std::endl;
        return 1;
    }
    PS4eye ps4eyeDevice(devicesPath[0], PS4eye::PS4EYE_RESOLUTION_1280x800, PS4eye::PS4EYE_FPS_60);
    static struct timeb start, end;

    uint countFrames = 0;
    double avgFps = 0;
    while (true) {
        ftime(&start);

        cv::imshow("Show PS4eye stereo image", ps4eyeDevice.grabFrame());
        int key = cv::waitKey(1);

        if (key != -1 || !ps4eyeDevice.isActive())
            break;
        ftime(&end);
        avgFps += 1.0 / ((end.time - start.time) + (end.millitm - start.millitm) * 0.001);

        ++countFrames;
        std::cout << "FPS = " << avgFps / countFrames << "\r";
        std::cout.flush();

    }

    return 0;
}
