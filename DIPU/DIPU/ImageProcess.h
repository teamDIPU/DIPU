#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include <math.h>
#include <iostream>

//#define RNG rng(12345);
#define daeunDebug 1
//
#include "use_opencv.h"

#define FILE "testImage/test2.png"
#define LimitDistanceEQU2PT 0.5
#define LimitDistancePT2PT 2

//선길이 최소 6mm (0.29*5) 이상
#define MinimumContourPixel 5

#define CannyThresh 100
#define CannyImageBrightness 11
#define CannyImageBrightnessTerm 1
#define A4Y double(210.0)
#define A4X double(297.0)
#define A4RATIO double(297/210)

#define scale 0.8

#define B 0
#define G 1
#define R 2

#define ColorNum 4




using namespace cv;
using namespace std;

class DIPU
{

public:
	Mat A_drawing;
	//int ImageProcess();
	vector<vector<Point2d>> ImageProcess();
	int ImageProcess_video();

	Mat getTargetMat();

	void myDrawContours(InputOutputArray image, vector<vector<Point>> contours, bool Dot=0);



	vector<vector<Point>> ContourApproximation(Mat src);
	vector<vector<Point2d>> ContoursTransform(Mat src, vector<vector<Point>> contours);




	Mat ColorTransform(Mat src);

	//void DisplayImage(int IDC_PICTURE_TARGET, Mat targetMat);


	int test();
	Point2f FindLinearEquation(Point Pt_A, Point Pt_B);
	float FindDistance_equ2pt(Point2f equ, Point Pt);
	float FindDistance_pt2pt(Point Pt_A, Point Pt_B);

private:
	;
	
};