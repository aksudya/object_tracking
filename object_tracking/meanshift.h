#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"  
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

class meanshift
{
public:

	meanshift();

	Mat src_1;
	Mat src_2;
	Rect rect_1;
	Rect rect_2;

	Mat Back_Projection1;
	Mat Back_Projection2;

	Mat dstHist;

	float dx;
	float dy;

	int maxHist;


	void Init(Mat img);
	void LoadImage(Mat img);
	void Caculate_Back_Projection();
	void Caculate_rect2();
};

