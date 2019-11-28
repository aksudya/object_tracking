// object_tracking.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "meanshift.h"
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
	Mat src;
	for (int i = 0; i <= 200; ++i)
	{
		std::ostringstream os;
		os << setw(4) << setfill('0') << i;
		string path = "./car2/" + os.str() + ".jpg";
		src = imread(path);
		//cvtColor(src, gray, CV_BGR2GRAY);


		

	}
}

