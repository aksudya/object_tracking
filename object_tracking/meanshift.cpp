#include "meanshift.h"

meanshift::meanshift()
{
	
}

void meanshift::Init(Mat img)
{
	src_1 = img.clone();
	src1_rect = src_1(rect_1);
	
}

void meanshift::LoadImage(Mat img)
{
	src_2 = img.clone();
	src2_rect = src_2(rect_1);
	Mat E(src2_rect.rows, src2_rect.cols, CV_32F);
	Epanechnikov_kernal = E.clone();
	CaculateKernal();
}

void meanshift::Caculate_Back_Projection()
{
	
}

void meanshift::Caculate_rect2()
{
	
}

void meanshift::Cacu_Hist()
{

}

void meanshift::CaculateKernal()
{
	int h = Epanechnikov_kernal.rows;
	int w = Epanechnikov_kernal.cols;

	float epanechnikov_cd = 0.1 * PI * h * w;
	float kernel_sum = 0.0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			float x = static_cast<float>(i - h / 2);
			float  y = static_cast<float> (j - w / 2);
			float norm_x = x * x / (h * h / 4) + y * y / (w * w / 4);
			float result = norm_x < 1 ? (epanechnikov_cd * (1.0 - norm_x)) : 0;
			Epanechnikov_kernal.at<float>(i, j) = result;
			kernel_sum += result;
		}
	}
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{		
			Epanechnikov_kernal.at<float>(i, j) /= kernel_sum;
		}
	}
	//return kernel_sum;
}
