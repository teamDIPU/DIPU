#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include <math.h>
#include <iostream>



//#define RNG rng(12345);
#define Debug 0
//
#include "use_opencv.h"
#include "opencv2/opencv.hpp"

#define FILE "testImage/test3.png"

#define LimitDistanceEQU2PT 0.5
#define LimitDistancePT2PT 2

//선길이 최소 6mm (0.29*5) 이상
#define MinimumContourPixel 5

#define CannyThresh 50 //100
#define CannyImageBrightness 4
#define CannyImageBrightnessTerm 0.5
#define A4Y double(210.0)
#define A4X double(297.0)
#define A4RATIO double(297/210)

#define scale 0.8

#define B 0
#define G 1
#define R 2

#define H 0
#define S 1
#define V 2

#define ColorNum 4


using namespace cv;
using namespace std;

class DIPU
{

public:
	DIPU();

	bool webcamMode = true;// true;
	CascadeClassifier face_classifier;
	Mat A_drawing;
	Mat cam_frame;
	//extern int facePosition;
	vector<vector<Point2d>> finalContours;

	//int ImageProcess();
	vector<vector<Point2d>> ImageProcess();
	void thinning(Mat edge);
	void setWebcamMode(bool mode);
	void myDrawContours(InputOutputArray image, vector<vector<Point>> contours, bool Dot = 0);

	Mat getTargetMat();
	Mat capture(VideoCapture capture);
	Mat ColorTransform(Mat src, Mat balck, Mat red, Mat skin);
	Mat ColorTransform_HLS(Mat src);
	Mat bright(Mat src, Mat bright1, Mat bright2);
	Mat pattern(Mat src, char * patternFile);

	vector<vector<Point2d>> colorTest();
	vector<vector<Point2d>> intensityContours(Mat src, Mat & dst);
	vector<vector<Point>> ContourApproximation(Mat src, Mat& dst = Mat());
	vector<vector<Point2d>> ContoursTransform(Mat src, vector<vector<Point>> contours);
	Point2f FindLinearEquation(Point Pt_A, Point Pt_B);

	int getFacePosition();
	int test();
	float FindDistance_equ2pt(Point2f equ, Point Pt);
	float FindDistance_pt2pt(Point Pt_A, Point Pt_B);

private:
	;

};