﻿// object_tracking.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "meanshift.h"
#include "PF.h"
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	Mat src;
	Mat gray;

	meanshift m;
	PF p;

	for (int i = 1; i <= 490; ++i)
	{
		std::ostringstream os;
		os << setw(4) << setfill('0') << i;
		string path = "C:/Users/46749/Desktop/data/BlurFace/img/" + os.str() + ".jpg";
		src = imread(path);

		std::vector<Mat> channels;

		//cvtColor(src, gray, COLOR_BGR2GRAY);
		cvtColor(src, gray, COLOR_BGR2HSV);

		split(gray, channels);

 		if (i == 1)
		{
			//246	226	94	114		BF
			//76, 79, 64, 52		car2
			//196,51,139,194		trans
			//58,100,28,23			panda
			//227	207	122	99		BC2	
			 //198,214,34,81
			//450, 91, 31, 37

			p.rect_1.x = 246;
			p.rect_1.y = 226;
			p.rect_1.width = 94;
			p.rect_1.height = 114;
			p.Init(gray);


			m.rect_1.x = 246;
			m.rect_1.y = 226;
			m.rect_1.width = 94;
			m.rect_1.height = 114;
			m.Init(channels[0]);
		}
		else
		{
			/*for (int i = 0; i < 50; ++i)
			{
				m.LoadImage(channels[0]);
				m.Cacu_Hist();
				m.Caculate_Back_Projection();
				m.Caculate_rect2();
				m.src_1 = m.src_2.clone();
			}*/

			p.LoadImage(gray);

		}

		Mat src11 = src.clone();
		for (auto pp:p.particles)
		{
			cv::rectangle(src, pp.rect, cv::Scalar(0, 255, 0), 2, 8, 0);  // 画矩形框
		}

		cv::rectangle(src, p.rect_1, cv::Scalar(255, 0, 0), 2, 8, 0);  // 画矩形框
		
		imshow("a", src);
		imshow("b", src11(p.rect_1));
		waitKey(0);
	}
}

