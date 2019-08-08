#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>   
#include "opencv2/legacy/legacy.hpp" 
#include <cmath>
#include "myStruct.h"
#include <string>
#include <vector>
#include<time.h>
using namespace std;
using namespace cv;

void on_MouseCallBack(int event, int x, int y, int flags, void* params);//拖动鼠标左键标记对象，拖动鼠标右键标记背景
int main()
{
	Mat image = imread("C:/Users/Snow-Ning/Pictures/test400.jpg");
	Mat img = image.clone();

	namedWindow(g_sMainWindowName);
	// set mouse call back function
	setMouseCallback(g_sMainWindowName, on_MouseCallBack, (void*)&img);

	while (1)
	{
		imshow(g_sMainWindowName, img);

		if (waitKey(10) == 27)
		{
			break;
		}
	}

	for (int i = 0; i < g_vpLinePoints.size(); i++)//输出标记为对象的像素点的像素值
		cout << g_vpLinePoints[i];
	cout << endl << endl;
	/*for (int i = 0; i < g_backLinePoints.size(); i++)//输出标记为背景的像素点的像素值
		cout << g_backLinePoints[i];*/

	clock_t start, finish;
	double totaltime;
	start = clock();
	constructGraph(image);
	finish = clock();
	totaltime = (double)(finish - start);
	cout << "\n此程序的运行时间为" << totaltime << "毫秒！" << endl;
	system("pause");


}

void on_MouseCallBack(int event, int x, int y, int flags, void* params)
{

	Mat& img = *(Mat*)params;
	Mat hh;
	cvtColor(img, hh, CV_BGR2GRAY);

	switch (event)
	{
	case EVENT_MOUSEMOVE:
	{
		if (g_bLeftButtonDown || g_bRightButtonDown)
		{
			g_pCurrent = Point(x, y); // current mouse position
			int grayValue = hh.at<uchar>(g_pCurrent);
			// add current mouse position
			if (g_bLeftButtonDown)
			{
				line(img, g_pPrevious, g_pCurrent, Scalar(255, 0, 0), 1, 8);
				g_vpLinePoints.push_back(g_pCurrent);
				objPixelValue.push_back(grayValue);
			} // draw line on the input image
			else
			{
				line(img, g_pPrevious, g_pCurrent, Scalar(0, 255, 0), 1, 8);
				g_backLinePoints.push_back(g_pCurrent);
				bkgPixelValue.push_back(grayValue);
			}
			//line(g_mMaskImg, g_pPrevious, g_pCurrent, 255, 8, 8); // draw line on the mask image

			g_pPrevious = g_pCurrent; // after drawing, current mouse position should be previous mouse position of next movement
		}
	}
	break;
	case EVENT_LBUTTONDOWN:
	{
		g_bLeftButtonDown = true;

		g_pPrevious = Point(x, y);

		g_vpLinePoints.push_back(g_pPrevious);
		int grayValue = hh.at<uchar>(g_pPrevious);
		objPixelValue.push_back(grayValue);
	}
	break;
	case EVENT_LBUTTONUP:
	{
		g_bLeftButtonDown = false;
	}
	break;
	case EVENT_RBUTTONDOWN:
	{
		g_bRightButtonDown = true;

		g_pPrevious = Point(x, y);

		g_backLinePoints.push_back(g_pPrevious);
		int grayValue = hh.at<uchar>(g_pPrevious);
		bkgPixelValue.push_back(grayValue);
	}
	break;
	case EVENT_RBUTTONUP:
	{
		g_bRightButtonDown = false;
	}
	}

}






