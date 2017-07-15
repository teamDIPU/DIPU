//
#include "ImageProcess.h"

Mat src;

unsigned char PenColor[ColorNum][3] = { { 0,0,0 },{ 60,60,255 },{ 200,220,250 },{ 255,255,255 } };// �� �� �� ��


vector<vector<Point2d>> DIPU::ImageProcess()
{
	cout << "Image Process\n";

	char* filename = FILE;
	//char* filename = "testImage/apple.png";
	src = imread(filename, 0);		//���� �̹���
	Mat src_3c = imread(filename, 3);	//���� �̹���(3c)
	Mat edge = imread(filename, 0);		//edge �̹���
	Mat dst = imread(filename, 3);		//���� �̹���

	if (src.rows != 0)
		cout << "distance (point to point) : " << 210.0 * LimitDistancePT2PT / src.rows << "mm" << "\n\n";

	int edgeThresh = CannyThresh;

	if (src.empty()) {
		cout << "can not open" << endl;
		return Mat(1, 1, CV_8UC1);
	}

	GaussianBlur(src, src, Size_<int>(5, 5), 1);

	///�ܰ��� ����
	// ��� ���� ���߾� canny edge detect
	while (1) {
		Canny(src, edge, edgeThresh, edgeThresh * 3, 3);
		Scalar a = ((mean(edge)));
		//cout << a[0] << "     " << edgeThresh << endl;
		if (a[0] < CannyImageBrightness - CannyImageBrightnessTerm) edgeThresh--;
		else if (a[0] > CannyImageBrightness + CannyImageBrightnessTerm) edgeThresh++;
		else {
			cout << "edge threshold : " << edgeThresh << endl;
			break;
		}
	}

	cv::imshow("edge Image", edge);
	cv::Mat erode1, erode2, erode3, erode4;
	cv::Mat element1(2, 2, CV_8U, cv::Scalar(1));
	element1.at<unsigned char>(0, 0) = 0;

	cv::Mat element2(2, 2, CV_8U, cv::Scalar(1));
	element2.at<unsigned char>(1, 0) = 0;

	cv::Mat element3(2, 2, CV_8U, cv::Scalar(1));
	element3.at<unsigned char>(0, 1) = 0;

	cv::Mat element4(2, 2, CV_8U, cv::Scalar(1));
	element4.at<unsigned char>(1, 1) = 0;

	cv::morphologyEx(edge, erode1, cv::MORPH_ERODE, element1, Point(1, 1));
	cv::morphologyEx(edge, erode2, cv::MORPH_ERODE, element2, Point(1, 0));
	//edge = edge - closed;
	cv::morphologyEx(edge, erode3, cv::MORPH_ERODE, element3, Point(0, 1));
	cv::morphologyEx(edge, erode4, cv::MORPH_ERODE, element4, Point(0, 0));
	edge = edge - erode1 - erode2;

	cv::namedWindow("Closed Image");
	cv::imshow("Closed Image", edge);


	// Contour approximation and Conversion 
	vector<vector<Point>> contours;
	vector<vector<Point2d>> TransformContours;
	contours = ContourApproximation(edge);
	TransformContours = ContoursTransform(edge, contours);

	/////���� ����
	////pyrMeanShiftFiltering(src_3c, color_3c, 25, 70, 2);
	//color_3c=ColorTransform(src_3c);
	//cvtColor(edge, edge_3c, CV_GRAY2BGR);
	////subtract(color_3c, edge_3c, dst);
	//subtract(color_3c, A_drawing, dst);

	imshow("source image", src_3c);
	//imshow("edge", edge);
	//imshow("color", color_3c);

	imshow("contour image", A_drawing);

	cout << "\n********** Image Process end **********\n\n";

	//return 0;
	return TransformContours;
}


int DIPU::test()
{
	Mat image = imread(FILE, 3);

	Mat color = ColorTransform(image);

	imshow("dst", color);
	return 0;
}

vector<vector<Point>> DIPU::ContourApproximation(Mat src)
{
	RNG rng(12345);
	Mat dst = src.clone();

	//imshow("sourc", src);

	if (src.empty()) {
		//cout << "can not open" << endl;
		//return 1;
	}

	//RNG rng(12345);
	// Mat output;
	vector<vector<Point>> contours;
	vector<vector<Point>> DividedContours;
	vector<vector<Point>> ApproximatedContours;
	vector<Vec4i> hierarchy;

	findContours(src, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, Point(0, 0));

	// Draw contours
	cout << "Draw contours**********\n";
	int CountourNum = 0;
	Mat drawing = Mat::zeros(src.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 1, 8, hierarchy, 0, Point());
		CountourNum += contours[i].size();

		//	cout << "contour " << i + 1 << "   size : " << contours[i].size() << endl;
	}
	cout << "Contour point : " << CountourNum << endl;
	cout << "countour number : " << contours.size() << endl << endl;



	// Check Overlap
	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size(); j++) {
			bool overlap = 0;
			for (int k = 0; k < j; k++) {
				if ((contours[i][j].x == contours[i][k].x) && (contours[i][j].y == contours[i][k].y)) overlap = 1;
			}
			if (overlap == 1) {
				overlap = 0;
				contours[i][j].x = contours[i][j].y = 0;
			}
		}
	}


	// Make Contours (DividedContours)
	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size(); j++) {
			vector<Point> it1;

			for (j; j < contours[i].size(); j++) {
				if ((contours[i][j].x == 0) && (contours[i][j].y == 0)) break;
				else {
					Point point(contours[i][j].x, contours[i][j].y);
					it1.push_back(point);
				}
			}
			if (it1.size() >MinimumContourPixel) DividedContours.push_back(it1);
		}
	}
	int DividedCountourNum = 0;
	// Check contours(remove overlap)
	cout << endl << "Divided Contour **********" << endl;
	for (int i = 0; i < DividedContours.size(); i++) {
		//cout << "contour " << i + 1 << "   size : " << DividedContours[i].size()<<endl ;
		DividedCountourNum += DividedContours[i].size();
		for (int j = 0; j < DividedContours[i].size(); j++) {
			//cout << DividedContours[i][j] << endl;
		}
	}
	cout << "Divided Contour point : " << DividedCountourNum << endl;
	cout << "countour number : " << DividedContours.size() << endl << endl;
	//cout << endl;

	// Polygun Approximation 
	for (int i = 0; i < DividedContours.size(); i++) {

		//cout << "Approximation contour" << i << endl << endl;
		vector<Point> it1;
		Point2f EquationPT2PT;
		float DistanceEQU2PT = 0, DistancePT2PT;
		int num = 0;
		for (int j = 0; j < DividedContours[i].size(); j++) {
			Point point(DividedContours[i][j].x, DividedContours[i][j].y);

			// ù ��° ����Ʈ, ������ ����Ʈ ����
			if (j == 0) {
				it1.push_back(point);
				continue;
			}
			else if (j == DividedContours[i].size() - 1) {
				it1.push_back(point);
				continue;
			}

			// �ƴ� ��� ������ �������� ���ϰ� ���� ���� ���� �� ������ �Ÿ��� ���Ѵ�.
			else {
				EquationPT2PT = FindLinearEquation(it1[it1.size() - 1], point);
				DistancePT2PT = FindDistance_pt2pt(it1[it1.size() - 1], point);
				if (DistancePT2PT > LimitDistancePT2PT) goto save;
				//cout << "Points : " << it1[it1.size() - 1] << point << EquationPT2PT << endl;
			}
			for (int k = num; k < j; k++) {
				DistanceEQU2PT = FindDistance_equ2pt(EquationPT2PT, DividedContours[i][k]);
				if (DistanceEQU2PT > LimitDistanceEQU2PT) goto save;
				//cout <<"Distance : "<< DividedContours[i][k] << DistanceEQU2PT <<endl;
			}
			continue;

		save:
			//cout << "save Point" << point << endl << endl;
			num = j;
			it1.push_back(point);
			continue;
		}
		//if (it1.size() !=0 ) ApproximatedContours.push_back(it1);

		if (it1.size() >MinimumContourPixel / LimitDistancePT2PT) ApproximatedContours.push_back(it1);
	}

	// Check contours(approximation)
	cout << endl << "Approximated Contour ********" << endl;
	int  ApproximatedCountourNum = 0;
	for (int i = 0; i < ApproximatedContours.size(); i++) {

#if daeunDebug
		cout << "contour " << i + 1 << "   size : " << ApproximatedContours[i].size() << endl;
#endif
		ApproximatedCountourNum += ApproximatedContours[i].size();
		for (int j = 0; j < ApproximatedContours[i].size(); j++) {

#if daeunDebug
			cout << ApproximatedContours[i][j] << endl;
#endif

		}
	}
	cout << "Approximated Contour point : " << ApproximatedCountourNum << endl;
	cout << "countour number : " << ApproximatedContours.size() << endl << endl;
	//cout << endl;

	//DividedContours
	//Mat T_drawing = Mat::zeros(src.size(), CV_8UC3);
	//for (int i = 0; i < DividedContours.size()-1; i++)
	//{
	//	myDrawContours(T_drawing, DividedContours);
	//}

	//ApproximatedContours
	A_drawing = Mat::zeros(src.size(), CV_8UC3);
	for (int i = 0; i < ApproximatedContours.size() - 1; i++)
	{
		myDrawContours(A_drawing, ApproximatedContours, 0);
	}

	return ApproximatedContours;

}

vector<vector<Point2d>> DIPU::ContoursTransform(Mat src, vector<vector<Point>> contours)
{
	// Convert form pixel to mm
	int TransformMode = 0;
	double ImageRatio = src.cols / src.rows;
	if (ImageRatio > A4RATIO) { cout << "���α��� ���߱�*************\n"; } //���α��̿� ���߱�
	else { cout << "���α��� ���߱�***********\n"; } //���α��̿� ���߱�

	vector<vector<Point2d>> TransformContour;
	for (int i = 0; i < contours.size(); i++) {
		vector<Point2d> it1;
		//cout << "\ncontour " << i + 1 << endl;

#if daeunDebug
		cout << "Transform Contour " << i << endl;
#endif
		for (int j = 0; j < contours[i].size(); j++) {

			if (ImageRatio > A4RATIO)//���α��̿� ���߱�
			{
				Point2d point(contours[i][j].x *(A4X / src.cols) * scale, (contours[i][j].y*(A4X / src.cols) + (A4Y - A4X*src.rows / src.cols) / 2) * scale);

				it1.push_back(point);
#if daeunDebug
				cout << "Transform Contour" << point << endl;
#endif
			}

			else //���α��̿� ���߱�
			{
				//Point2d point(contours[i][j].x * (A4Y/src.rows)+(A4X)*(1-A4Y/src.cols)/2 , contours[i][j].y*(A4Y/src.rows));
				Point2d point((contours[i][j].x * (A4Y / src.rows) + (A4X - A4Y * src.cols / src.rows) / 2)*scale, contours[i][j].y*(A4Y / src.rows)*scale);

				it1.push_back(point);
#if daeunDebug
				cout << "Transform Contour" << point << endl;
#endif
			}
		}
		if (it1.size() > 0) {
			TransformContour.push_back(it1);
		}
	}

	return TransformContour;
}

Point2f DIPU::FindLinearEquation(Point Pt_A, Point Pt_B)
{
	Point2f equ;
	float m = 500;
	if ((Pt_A.x) != Pt_B.x) m = ((float(Pt_A.y) - Pt_B.y) / (float(Pt_A.x) - Pt_B.x)); //����
	float n = Pt_A.y - m*Pt_A.x; //����
	equ.x = m;
	equ.y = n;
	return equ;
}

float DIPU::FindDistance_equ2pt(Point2f equ, Point Pt)
{
	return(abs(equ.x*Pt.x - Pt.y + equ.y) / sqrt(equ.x*equ.x + 1));
}

float DIPU::FindDistance_pt2pt(Point Pt_A, Point Pt_B)
{
	return sqrt((Pt_A.x - Pt_B.x)*(Pt_A.x - Pt_B.x) + (Pt_A.y - Pt_B.y)*(Pt_A.y - Pt_B.y));
}

void DIPU::myDrawContours(InputOutputArray image, vector<vector<Point>> contours, bool Dot)
{
	RNG rng(12345);
	for (int i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		for (int j = 1; j < contours[i].size(); j++) {
			Point pt1(contours[i][j - 1].x, contours[i][j - 1].y), pt2(contours[i][j].x, contours[i][j].y);
			line(image, pt1, pt2, color);
			if (Dot == 1) {
				line(image, pt1, pt1, Scalar(255, 255, 255), 2);
				line(image, pt2, pt2, Scalar(255, 255, 255), 2);
			}
		}
	}
}

int DIPU::ImageProcess_video()
{
	cout << "Image Process for Video";

	CvCapture* capture = NULL;
	IplImage* grabImage;

	cvGrabFrame(capture);
	grabImage = cvRetrieveFrame(capture);

	// else if (grabImage == NULL) {
	// }

	return 0;


}

Mat DIPU::getTargetMat() {
	if (A_drawing.empty()) return src;
	return A_drawing;
}

//////////////////////////////////////////////////
//color
Mat DIPU::ColorTransform(Mat src)
{
	Mat color = src.clone();
	pyrMeanShiftFiltering(src, color, 25, 70, 2);

	TypedMat<unsigned char> tmSrc = src;
	TypedMat<unsigned char> tmColor = color;

	unsigned int distance = 0xffffffff;
	int ColorIndex = 0;

	for (int i = 0; i < src.cols; i++) {
		for (int j = 0; j < src.rows; j++) {
			for (int k = 0; k < ColorNum; k++) {
				int blue = (tmSrc(j, i, B) - PenColor[k][B]);
				int green = (tmSrc(j, i, G) - PenColor[k][G]);
				int red = (tmSrc(j, i, R) - PenColor[k][R]);

				unsigned int squre = (blue*blue + green*green + red*red);

				if (distance > squre) {
					distance = squre;
					ColorIndex = k;
				}
			}

			tmColor(j, i, B) = PenColor[ColorIndex][B];
			tmColor(j, i, G) = PenColor[ColorIndex][G];
			tmColor(j, i, R) = PenColor[ColorIndex][R];
			distance = 0xffffffff;
		}
	}
	return color;
}