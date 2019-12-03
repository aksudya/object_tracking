#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"  
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define NUMBINS 16
#define COLORWITH 256
#define PI acos(-1)

using namespace cv;

class Particle
{
public:
	float x;
	float y;
	float vx;
	float vy;
	float hx;
	float hy;
	float a;
	float weight;

	float hist[NUMBINS];

	Particle();
};

class PF
{
public:
	Mat src1;
	Mat src2;

	Rect rect_1;

	Mat src1_rect;
	Mat src2_rect;

};

