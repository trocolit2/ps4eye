/*
 * stereo_test.cpp
 *
 *  Created on: Sep 30, 2015
 *      Author: tiagotrocoli
 */

#include <PS4eye.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sys/timeb.h>

int main(int argc, char **argv) {

    std::vector<std::string> devicesPath;
    PS4eye::ps4eyeDevices(&devicesPath);

    if (devicesPath.empty()) {
        std::cout << "NO PS4EYE DEVICE FOUND" << std::endl;
        return 1;
    }

    cv::Rect rightROI(0, 0, 640, 400);
    cv::Rect leftROI(640, 0, 640, 400);
    PS4eye ps4eyeDevice(devicesPath[0], PS4eye::PS4EYE_RESOLUTION_640x400, PS4eye::PS4EYE_FPS_30);
    static struct timeb start, end;

    uint countFrames = 0;
    double avgFps = 0;
    int key = -1;
    cv::Mat frame, left, right, stereo_image;

    cv::StereoBM stereo(cv::StereoBM::BASIC_PRESET);
//    cv::StereoSGBM stereo;
    while (true) {
        ftime(&start);

        frame = ps4eyeDevice.grabFrame();
        cv::cvtColor(frame(leftROI), left, CV_BGR2GRAY);
        cv::cvtColor(frame(rightROI), right, CV_BGR2GRAY);

        stereo(left, right, stereo_image, CV_16S);
//        stereo(left, right, stereo_image);

        cv::imshow("left", left);
        cv::imshow("right", right);
        cv::imshow("disparity", stereo_image);

        key = cv::waitKey(1);

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
