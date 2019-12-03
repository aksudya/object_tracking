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

void meanshift::Caculate_Back_Projection()		//权重（反向投影）
{
	cv::Mat weight(src1_rect.rows, src1_rect.cols, CV_32F, cv::Scalar(1.0000));
	Back_Projection1 = weight.clone();

	for (int i = 0; i < src1_rect.rows; i++)
	{
		//col_index = rec.x;
		for (int j = 0; j < src1_rect.cols; j++)
		{
			int curr_pixel = src2_rect.at<uchar>(i, j);
			int bin_value = curr_pixel / (COLORWITH / NUMBINS);
			Back_Projection1.at<float>(i, j) *= sqrt(hist1[bin_value]/hist2[bin_value]);
			//col_index++;
		}
	}
}

void meanshift::Caculate_rect2()
{
	float centrei = static_cast<float>(src1_rect.rows / 2.0);
	float centrej = static_cast<float>(src1_rect.cols / 2.0);

	float delta_x = 0;
	float delta_y = 0;
	float sum_wij = 0;
	for (int i = 0; i < src1_rect.rows; i++)
	{
		for (int j = 0; j < src1_rect.cols; j++)
		{
			float norm_i = (float)(i - centrei) / (float)centrei;
			float norm_j = (float)(j - centrej) / (float)centrej;
			float mult = (norm_i*norm_i + norm_j*norm_j) > 1.0 ? 0.0 : 1.0;
			
			delta_x += (float)norm_j * Back_Projection1.at<float>(i, j) * mult;
			delta_y += (float)norm_i * Back_Projection1.at<float>(i, j) * mult;
			sum_wij += (float)Back_Projection1.at<float>(i, j) * mult;
		}
	}

	rect_1.x += (delta_x / sum_wij) * centrej;
	rect_1.y += (delta_y / sum_wij) * centrei;

}

void meanshift::Cacu_Hist()
{
	for (int i = 0; i < NUMBINS; ++i)
	{
		hist1[i] = 1e-10;
		hist2[i] = 1e-10;
	}

	for (int i = 0; i < src1_rect.rows; ++i)
	{
		for (int j = 0; j < src1_rect.cols; ++j)
		{
			int index1 = src1_rect.at<uchar>(i,j)/ (COLORWITH / NUMBINS);
			hist1[index1] += Epanechnikov_kernal.at<float>(i, j);

			int index2 = src2_rect.at<uchar>(i, j) / (COLORWITH / NUMBINS);
			hist2[index2] += Epanechnikov_kernal.at<float>(i, j);
		}
	}
}

void meanshift::CaculateKernal()		//核函数
{
	int h = Epanechnikov_kernal.rows;
	int w = Epanechnikov_kernal.cols;

	float epanechnikov_cd = 0.1 * PI * h * w;
	float kernel_sum = 0.0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			float x = i - h / 2.0;
			float  y =  j - w / 2.0;
			float norm_x = x * x / (h * h / 4.0) + y * y / (w * w / 4.0);
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
