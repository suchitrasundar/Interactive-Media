//
//  main_tutorial.cpp
//  FirstOpenCV
//
//  Created by Ju Shen on 7/28/18.
//  Copyright © 2018 Ju Shen. All rights reserved.
//



#include <stdio.h> //create a file, this is a default library

//printf("Hello World\n"); //output
//scanf()  //input


#include <iostream> //system library

//cout<< "Hello World"<<endl;
//cin>> ...

#include <opencv2/core/core.hpp> //include all the basic data types from opencv
#include <opencv2/highgui/highgui.hpp>//allow you to load image from computer or display image onto the screen
#include <opencv2/imgproc/imgproc.hpp> //provide functions to process an image

using namespace cv; //tell the compiler most of the data types involved are from OpenCV
using namespace std; //C++ library


Mat canvas, im;

Point p1, p2, p3, p4;

int counter = 0; //record user's click times
int index = 0;
vector<Point> pt_hit;
const vector<int> parameter = { CV_IMWRITE_JPEG_QUALITY, 100 };



void drawSpline()
{
	
	//printf("Draw Spline is called!\n");

	//Write code to create the three matrices: M, G, and A
	double M_data[4][4] = { {-1, 3, -3, 1}, {3, -6, 3, 0}, {-3, 3, 0, 0}, {1, 0, 0, 0} };
	Mat M = Mat(4, 4, CV_64FC1, M_data);

	double G_data[4][2] = { {(double)p1.x, (double)p1.y}, {(double)p2.x, (double)p2.y}, {(double)p3.x, (double)p3.y}, 
	{(double)p4.x, (double)p4.y} };
	Mat G = Mat(4, 2, CV_64FC1, G_data);

	Mat A = M * G;


	//write a for-loop of code to draw the spline on the image
	for (double t = 0.0; t < 1.0; t += 0.001)
	{
		double x = A.ptr<double>(0)[0] * t * t * t
			+ A.ptr<double>(1)[0] * t * t
			+ A.ptr<double>(2)[0] * t
			+ A.ptr<double>(3)[0];

		double y = A.ptr<double>(0)[1] * t * t * t
			+ A.ptr<double>(1)[1] * t * t
			+ A.ptr<double>(2)[1] * t
			+ A.ptr<double>(3)[1];

		circle(canvas, Point(x, y), 1, Scalar(0, 0, 0), 1);

	}

}


void myMouse(int event, int x, int y, int, void*)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		circle(canvas, Point(x, y), 1, Scalar(0, 0, 255), 3);

		counter++;
		if (counter % 4 == 0)
		{
			p4.x = x;
			p4.y = y;
			drawSpline();
			line(canvas, Point(p1.x, p1.y), Point(p2.x, p2.y), Scalar(255, 0, 0), 2, 8);
			line(canvas, Point(p3.x, p3.y), Point(p4.x, p4.y), Scalar(255, 0, 0), 2, 8);
		}

		else if (counter % 4 == 1) //even number click
		{
			p1.x = x;
			p1.y = y;


		}
		else if (counter % 4 == 2)
		{
			p2.x = x;
			p2.y = y;

		}
		else if (counter % 4 == 3)
		{
			p3.x = x;
			p3.y = y;


		}
		pt_hit.push_back(p1);
		pt_hit.push_back(p2);
		pt_hit.push_back(p3);
		pt_hit.push_back(p4);

	}

	//dragging control points

	if (event == CV_EVENT_LBUTTONDOWN)
	{

		if (Point(x,y) == p1 ||Point(x,y) == p2||Point(x,y)==p3||Point(x,y)==p4)
		{
			pt_hit[index].x = x;
			pt_hit[index].y = y;
		}
		else
		{
			pt_hit.push_back(Point(x, y));
		}

		for (int i = 0; i < pt_hit.size(); i++)
		{
			if ((x - pt_hit[i].x) + (y - pt_hit[i].y) <= 10)
			{
				index = i;
				break;
			}
		}
	}
}



int main()
{
	Mat image_save;
	canvas = Mat(800, 1000, CV_8UC3); //create an image 1000x800
	canvas = Scalar(255, 255, 255); //make it a white image
	// "=" will use the scalar as input argument to initialize the Mat object "canvas"


	namedWindow("painting"); //declare the window first if a mouse event needs to be registered
	setMouseCallback("painting", myMouse);

	while (1)
	{
		
		imshow("painting", canvas);
		char c = waitKey(1);
		switch (c)
		{
		case 's':
		case 'S':

			imwrite("test.jpg", canvas);
	
		case 'r':
		case 'R':
			pt_hit.pop_back();

		}

		if (c == 27)
			break;
	}

	return 0;
}


