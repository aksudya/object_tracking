#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"  
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <random>

#define NUMBINS 16
#define COLORWITH 256
#define PI acos(-1)

#define N_PARTICLES 100
#define POS_VAR		20
#define SCALE_VAR   30
#define BETA		0.8
#define ALPHA		0.8

using namespace cv;
using namespace std;

class Particle
{
public:

	Rect rect;

	float vx;
	float vy;
	//float scale;

	float weight;

	float sum_weight;

	Mat img;

	Mat hist;

	Particle();
};

class PF
{
public:
	Mat src1;
	Mat src2;

	Rect rect_1;
	Rect pre_rect;

	Vec2f predict;

	Mat src1_rect;
	Mat src2_rect;

	Mat ref_hist;

	vector<Particle> particles;

	default_random_engine rnd_e;

	PF();

	void Init(Mat img);
	void LoadImage(Mat img);

	void TraslateParticls();
	void Updatepredict();
	void Updateweight();
	void UpdateHist();
	void UpdateRect();

	Mat CaculateHist(Mat img);
	Mat CaculatKernal(int rows, int cols);
};

