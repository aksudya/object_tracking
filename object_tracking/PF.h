#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"  
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define NUMBINS 16
#define COLORWITH 256
#define PI acos(-1)

#define N_PARTICLES 100

using namespace cv;
using namespace std;

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

	Mat hist;

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

	vector<Particle> particles;

	void Init(Mat img);
	void LoadImage(Mat img);

};

