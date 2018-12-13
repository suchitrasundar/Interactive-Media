#include<stdio.h>
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;
Mat frame, dup;
char click;
Rect roi;
Point point1, point2;
int avgRGB[3], sumRGB[3];
int blurDegree = 5;
int drag = 0;

void myMouse(int event, int x, int y, int flags, void* userdata)

{
	if (event == CV_EVENT_LBUTTONDOWN && !drag)

	{
		point1 = Point(x, y);
		drag = 1;
		roi = Rect(point1.x, point1.y, x - point1.x, y - point1.y);
	}
	if (event == CV_EVENT_MOUSEMOVE && drag)
	{
		Mat img = frame.clone();
		point2 = Point(x, y);
		rectangle(img, point1, point2, Scalar(255, 0, 0), 3, 8, 0);
		imshow("My_Window", img);

	}

	if (event == CV_EVENT_LBUTTONUP && drag)
	{

		point2 = Point(x, y);
		drag = 0;
		roi = Rect(point1.x, point1.y, x - point1.x, y - point1.y);
	}

	if (event == CV_EVENT_LBUTTONUP)
	{
		drag = 0;
		roi = Rect(point1.x, point1.y, x - point1.x, y - point1.y);
	}
}
void findAverage(const int x, const int y)
{
	for (int i = 0; i < blurDegree; i++)
	{
		for (int j = 0; j < blurDegree; j++)
		{
			if ((y + i) < dup.rows && (x + j) < dup.cols)
			{
				sumRGB[0] += dup.ptr<uchar>(y + i)[3 * (x + j)+0];
				sumRGB[1] += dup.ptr<uchar>(y + i)[3 * (x + j) + 1];
				sumRGB[2] += dup.ptr<uchar>(y + i)[3 * (x + j) + 2];
			}
		}
	}

	for (int k = 0; k < 3; k++)
		avgRGB[k] = sumRGB[k] / (blurDegree * blurDegree);
} 

void blurArea(const int x, const int y)
{
	for (int i = 0; i < blurDegree; i++)
	{
		for (int j = 0; j < blurDegree; j++)
		{
			if ((y + i) < frame.rows && (x + j) < frame.cols)
			{
				frame.ptr<uchar>(y + i)[3 * (x + j)+0] = avgRGB[0];
				frame.ptr<uchar>(y + i)[3 * (x + j) + 1] = avgRGB[1];
				frame.ptr<uchar>(y + i)[3 * (x + j) + 2] = avgRGB[2];
			}
		}
	}
} 


void blur()
{
	for (int i = point1.y; i < point2.y; i += blurDegree)
	{
		for (int j = point1.x; j < point2.x; j += blurDegree)
		{
			findAverage(j, i);
			blurArea(j, i);

			for (int k = 0; k < 3; k++)
				avgRGB[k] = sumRGB[k] = 0;
		}
	}
} 

int main(int argc, char** argv)
{
	VideoCapture camera(0); //0 means the first camera
	if (!camera.isOpened())
	{
		cout << "Could not initialize capturing...\n";
		return 0;
	}
	namedWindow("My_Window");
	setMouseCallback("My_Window", myMouse, 0);
	while (1)
	{
		if (!camera.read(frame))
			break;
		dup = frame.clone();
		rectangle(frame, roi, Scalar(0, 0, 255), 3, 8, 0);
		blur();
		imshow("My_Window", frame);
		char c = waitKey(1); // let opencv listen to your keyboard event
		switch (c)
		{
		case 'i':
			if (blurDegree < 5)
			{
				blurDegree = blurDegree + 1;
			}
			else
			{
				blurDegree = blurDegree + 5;
			}
			break;

		case 'I':
			if (blurDegree < 5)
			{
				blurDegree = blurDegree + 1;
			}
			else
			{
				blurDegree = blurDegree + 5;
			}
			break;

		case 'd':
			if (blurDegree <= 5 && blurDegree > 1)
			{
				blurDegree = blurDegree - 1;
			}
			else
			{
				blurDegree = blurDegree - 5;
			}
			break;
		case 'D':
			if (blurDegree <= 5 && blurDegree > 1)
			{
				blurDegree = blurDegree - 1;
			}
			else
			{
				blurDegree = blurDegree - 5;
			}
			break;
		}


		if (c == 27) ////27 is the ASCII code for "ESc"
			break;
	}
}