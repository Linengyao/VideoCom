// Kmeans.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv.hpp>
using namespace cv;
using namespace std;

int createMaskByKmeans(cv::Mat src, cv::Mat & mask)
{
	if ((mask.type() != CV_8UC1)
		|| (src.size() != mask.size())
		) {
		return 0;
	}

	int width = src.cols;
	int height = src.rows;

	int pixNum = width * height;
	int clusterCount = 2;
	Mat labels;
	Mat centers;

	//制作kmeans用的数据
	Mat sampleData = src.reshape(3, pixNum);
	Mat km_data;
	sampleData.convertTo(km_data, CV_32F);

	//执行kmeans
	TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 0.1);
	kmeans(km_data, clusterCount, labels, criteria, clusterCount, KMEANS_PP_CENTERS, centers);

	//制作mask
	uchar fg[2] = { 0,255 };
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			mask.at<uchar>(row, col) = fg[labels.at<int>(row*width + col)];
		}
	}
	if (mask.at<uchar>(0, 0) == 255)
	{
		//反转整个图像黑白,防止闪烁
		mask = 255 - mask;
	}
	return 0;
}

int main()
{

	//实例化的同时初始化
	VideoCapture capturebg("E:\\课程\\大二下\\数字图像处理\\绿幕.mp4");//背景
	VideoCapture capture("E:\\课程\\大二下\\数字图像处理\\鸡.mp4");  //前景

	while (1) {
		Mat frame;	//存储每一帧的图像
		Mat framep;
		Mat result;
		Mat people;
		capturebg >> frame;	//读取背景
		capture >> framep; //读取前景

		resize(framep, people, frame.size());


		if ((frame.data != NULL) && (people.data != NULL)) {
			people.copyTo(result);
			Mat mask = Mat::zeros(frame.size(), CV_8UC1);
			createMaskByKmeans(frame, mask);


			for (int i = 0; i < frame.rows; i++)	
			{
				for (int j = 0; j < frame.cols; j++)	
				{
					if ((mask.at<uchar>(i, j) == 255))	
					{
						result.at<Vec3b>(i, j)[0] = frame.at<Vec3b>(i, j)[0];
						result.at<Vec3b>(i, j)[1] = frame.at<Vec3b>(i, j)[1];
						result.at<Vec3b>(i, j)[2] = frame.at<Vec3b>(i, j)[2];
					}
				}
			}



			//imshow("srcMat", frame);
			//imshow("mask", mask);
			imshow("result", result);
			waitKey(30);
		}
		else {
			cout << "the Video Ends!";
			break;
		}

	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
