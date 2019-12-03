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


	for (int i = 1; i <= 408; ++i)
	{
		std::ostringstream os;
		os << setw(4) << setfill('0') << i;
		string path = "./data/Bird1/img/" + os.str() + ".jpg";
		src = imread(path);

		std::vector<Mat> channels;

		cvtColor(src, gray, COLOR_BGR2GRAY);
		//cvtColor(src, gray, COLOR_BGR2HSV);

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
			m.rect_1.x = 450;
			m.rect_1.y = 91;
			m.rect_1.width = 31;
			m.rect_1.height = 37;
			m.Init(channels[0]);
		}
		else
		{
			for (int i = 0; i < 50; ++i)
			{
				m.LoadImage(channels[0]);
				m.Cacu_Hist();
				m.Caculate_Back_Projection();
				m.Caculate_rect2();
				m.src_1 = m.src_2.clone();
			}

		}

		cv::rectangle(src, m.rect_1, cv::Scalar(255, 0, 0), 2, 8, 0);  // 画矩形框
		
		imshow("a", src);
		imshow("b", src(m.rect_1));
		waitKey(0);
	}
}

