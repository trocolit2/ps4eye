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
#include <opencv2/contrib/contrib.hpp>
#include <iostream>
#include <sys/timeb.h>

int main(int argc, char **argv) {

    cv::Mat Q = (cv::Mat_<double>(4, 4) << 1., 0., 0., -3.1800573730468750e+02, 0., 1., 0., -2.0075852012634277e+02, 0., 0., 0., 4.1271443936532944e+02, 0., 0., 1.2566742303865070e+01, 0.);

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
    cv::Mat frame, left, right, dispMapBM, dispMapSGBM, dispImage, stereoImage, img3d;

    cv::StereoBM sbm;
    sbm.state->SADWindowSize = 7;
    sbm.state->textureThreshold = 100;
    sbm.state->uniquenessRatio = 10;
    sbm.state->speckleWindowSize = 1;
    sbm.state->speckleRange = 8;
    sbm.state->disp12MaxDiff = 1;

    cv::StereoSGBM sgbm;
    sgbm.SADWindowSize = 5;
    sgbm.numberOfDisparities = 192;
    sgbm.preFilterCap = 4;
    sgbm.minDisparity = -64;
    sgbm.uniquenessRatio = 1;
    sgbm.speckleWindowSize = 150;
    sgbm.speckleRange = 2;
    sgbm.disp12MaxDiff = 10;
    sgbm.fullDP = false;
    sgbm.P1 = 600;
    sgbm.P2 = 2400;

    while (true) {
        ftime(&start);

        frame = ps4eyeDevice.grabFrame();
        cv::cvtColor(frame(leftROI), left, CV_BGR2GRAY);
        cv::cvtColor(frame(rightROI), right, CV_BGR2GRAY);

        cv::imshow("stereo frame", frame);

        sbm(left, right, dispMapBM);
        cv::normalize(dispMapBM, dispImage, 0, 255, CV_MINMAX, CV_8U);
        cv::reprojectImageTo3D(dispMapBM, img3d, Q, true, CV_16S);
//        cv::normalize(img3d, dispImage, 0, 255, CV_MINMAX, CV_8U);
        cv::imshow("disparity BM", dispImage);
        cv::imshow("3d IMG", img3d);
//        cv::reprojectImageTo3D()

//        sgbm(left, right, dispMapSGBM);
//        cv::normalize(dispMapSGBM, dispImage, 0, 255, CV_MINMAX, CV_8U);
//        cv::imshow("disparity SGBM", dispImage);

//        cv::imshow("disparity", dispMap);
//        cv::applyColorMap(dispImage, stereoImage, cv::COLORMAP_JET);
//        cv::imshow("disparity", stereoImage);

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
