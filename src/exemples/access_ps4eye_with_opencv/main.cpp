#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;

char key;
int main(int argc, char** argv) {

	CvCapture* capture = cvCaptureFromCAM(1); //Capture using any camera connected to your system
	Mat imageTotal;
	Mat imageLeft;
	Mat imageRigth;
	while (1) { //Create infinte loop for live streaming

		Mat frame = cvQueryFrame(capture);
		//IplImage* frame = cvQueryFrame(capture); //Create image frames from capture
		Mat camLeft(frame, Rect(48, 0, 1280, 800));
		Mat camRigth(frame, Rect(1328, 0, 1280, 800));
		imageTotal = frame;
		imageLeft = camLeft;
		imageRigth = camRigth;
		imshow("Camera_total", frame);   //Show image frames on created window
		imshow("Camera_esquerda", camLeft);
		imshow("Camera_direita", camRigth);
		key = cvWaitKey(1);     //Capture Keyboard stroke
		if (char(key) == 27) {
			break;      //If you hit ESC key loop will break.
		}
	}
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	imwrite("frame.png", imageTotal, compression_params);
	imwrite("esquerda.png", imageLeft, compression_params);
	imwrite("direita.png", imageRigth, compression_params);

	//	imshow("Camera_direita", imageTotal);

	//waitKey(0);     //Capture Keyboard stroke

	cvReleaseCapture(&capture); //Release capture.
	cvDestroyWindow("Camera_Output"); //Destroy Window
	return 0;
}
