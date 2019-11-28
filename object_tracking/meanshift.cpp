#include "meanshift.h"

meanshift::meanshift()
{
	
}

void meanshift::Init(Mat img)
{
	src_1 = img.clone();
}

void meanshift::LoadImage(Mat img)
{
	src_2 = img.clone();
}

void meanshift::Caculate_Back_Projection()
{
	int histSize[] = { 256 };
	float midRanges[] = { 0, 256 };
	const float* ranges[] = { midRanges };
	Mat img = src_1(rect_1);
	calcHist(&img, 1, 0, Mat(), dstHist, 1, histSize, ranges, true, false);
	Mat img2 = src_2(rect_1);
	//Back_Projection2 = img2.clone();
	//Back_Projection1 = img.clone();

	//Mat Back2(rect_1.height, rect_1.width, CV_32F);
	Mat Back1(rect_1.height, rect_1.width, CV_32F);

	Back_Projection2 = Back1.clone();
	Back_Projection1 = Back1.clone();

	maxHist = 0;
	for (int i = 0; i < 256; ++i)
	{
		if(dstHist.at<float>(i)>maxHist)
		{
			maxHist = dstHist.at<float>(i);
		}
	}
	int centerx = rect_1.height / 2;
	int centery = rect_1.width / 2;
	//Back_Projection = src_2.clone();
	for (int i = 0; i < Back_Projection2.rows; ++i)
	{
		for (int j = 0; j < Back_Projection2.cols; ++j)
		{
			//img2.at<uchar>(i,j)
			float dis = ((i - centerx) * (i - centerx) + (j - centery) * (j - centery))/ (0.25 * (rect_1.height * rect_1.height + rect_1.width * rect_1.width));
			float weight = 1 - dis+FLT_MIN;

			Back_Projection2.at<float>(i, j) = weight*dstHist.at<float>(img.at<uchar>(i, j));
			Back_Projection1.at<float>(i, j) = weight * dstHist.at<float>(img2.at<uchar>(i, j));
			
			//Back_Projection2.at<uchar>(i, j) = sqrt((float)Back_Projection1.at<uchar>(i, j) / (float)Back_Projection2.at<uchar>(i, j));
		}
	}
	//cv::normalize(Back_Projection2, Back_Projection2, 0, 255, cv::NORM_MINMAX);
	//cv::normalize(Back_Projection1, Back_Projection1, 0, 255, cv::NORM_MINMAX);
}

void meanshift::Caculate_rect2()
{
	dx = 0;
	dy = 0;
	int centerx = rect_1.height / 2;
	int centery = rect_1.width / 2;
	Mat kk(Back_Projection2.rows, Back_Projection2.cols, CV_32F);
	double tweight = 0;
	for (int i = 0; i < Back_Projection2.rows; ++i)
	{
		for (int j = 0; j < Back_Projection2.cols; ++j)
		{
			//float dis = ((i - centerx) * (i - centerx)+ (j - centery) * (j - centery));
			float dis = ((i - centerx) * (i - centerx) + (j - centery) * (j - centery)) / (0.25 * (rect_1.height * rect_1.height + rect_1.width * rect_1.width));

			float p = sqrt((float)Back_Projection1.at<float>(i, j)/ Back_Projection2.at<float>(i, j));
			

			dx += 2 * sqrt(dis) * (float)(j - centery) * p;
			dy += 2 * sqrt(dis) * (float)(i - centerx) * p;



			tweight += 2 * sqrt(dis);
			kk.at<float>(i, j) = dy;
		}
	}

	dx /= tweight;
	dy /= tweight;

	//rect_2 = rect_1;
	rect_1.x += dx;
	rect_1.y += dy;

}
