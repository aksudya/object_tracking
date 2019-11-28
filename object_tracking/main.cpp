// object_tracking.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "meanshift.h"
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	Mat src;
	Mat gray;

	meanshift m;
	for (int i = 1; i <= 450; ++i)
	{
		std::ostringstream os;
		os << setw(4) << setfill('0') << i;
		string path = "./BF/" + os.str() + ".jpg";
		src = imread(path);
		cvtColor(src, gray, COLOR_BGR2GRAY);
		if (i == 1)
		{
			//246	226	94	114
			//76, 79, 64, 52
			m.rect_1.x = 246;
			m.rect_1.y = 226;
			m.rect_1.width = 94;
			m.rect_1.height = 114;
			m.Init(gray);
		}
		//m.Caculate_Back_Projection();
		else
		{
			m.LoadImage(gray);
			m.Caculate_Back_Projection();
			m.Caculate_rect2();
			m.src_2 = m.src_1.clone();
		}

		cv::rectangle(src, m.rect_1, cv::Scalar(255, 0, 0), 2, 8, 0);  // 画矩形框
		
		imshow("a", src);
		waitKey(0);
	}
}

