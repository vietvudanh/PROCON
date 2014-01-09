// Dice_Chenxiao.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include "opencv2/highgui/highgui.hpp"
 
using namespace cv;

int p1, p2, a1,a2;
Mat dice, canny;
IplImage *sample;

void view() {
    Canny(dice, canny, p1, p2);
	cvShowImage("Canny",&(IplImage)canny);
	sample=cvCreateImage(cvSize(4*sqrt(1000/3.2),4*sqrt(1000/3.2)),IPL_DEPTH_8U, 3);
	cvCircle( sample, Point(sqrt(a1/3.1),sqrt(a1/3.1)), sqrt(a1/3.1), Scalar(255,255,255), -1, 8, 0 );
	cvCircle( sample, Point(2*sqrt(a1/3.1)+sqrt(a2/3.2),sqrt(a2/3.2)), sqrt(a2/3.2), Scalar(255,255,255), -1, 8, 0 );
	cvShowImage("Sample",sample);

}

Mat redFilter(const Mat& src)
{
    assert(src.type() == CV_8UC3);

    Mat redOnly;
    inRange(src, Scalar(10, 10, 140), Scalar(50, 90, 255), redOnly);

    return redOnly;
}

IplImage* GetThresholdedImage(IplImage* imgHSV){       
       IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
       cvInRangeS(imgHSV, cvScalar(97,0,0), cvScalar(160,255,255), imgThresh);
       return imgThresh;
} 

int main(int argc, const char** argv)
{
	IplImage *src=cvLoadImage("dice.jpg", 1);
	
	cvSmooth(src, src, CV_GAUSSIAN,3,3);
    dice = Mat(src);
    p1 = 50;
    p2 = 200;
	a1 = 30;
    a2 = 500;

    Canny(dice, canny, 50, 200);
	cvShowImage("Canny",&(IplImage)canny);
	cvShowImage("Sample",sample);

    cvNamedWindow("Tool");
	cvNamedWindow("Sample");
	cvResizeWindow("Tool",400,300);
	cvCreateTrackbar("p1","Tool",&p1,1000);
	cvCreateTrackbar("p2","Tool",&p2,1000);
	cvCreateTrackbar("Min area","Tool",&a1,1000);
	cvCreateTrackbar("Max area","Tool",&a2,1000);
	textbox t1;
	int p1check=p1,p2check=p2,a1check=a1,a2check=a2;
    while (true){
		if (p1!=p1check||p2!=p2check||a1!=a1check||a2!=a2check){
			view();
			p1check=p1;
			p2check=p2;
			p1check=a1;
			p2check=a2;
		}
		int c = cvWaitKey(10);
		//If 'ESC' is pressed, break the loop
		if((char)c==27 )	
			break;
	}

    int num = 0;
    for(int y=0;y<canny.size().height;y++)
    {
        uchar *row = canny.ptr(y);
        for(int x=0;x<canny.size().width;x++)
        {
            if(row[x] <= 128)
            {
                int area = floodFill(canny, Point(x,y), CV_RGB(0,0,160));
                printf("filling %d, %d gray, area is %d\n", x, y, area);
                if(area>a1*0.8 && area < a2*1.2) {
					num++;
					circle( dice, Point(x+sqrt(area/3.4),y+sqrt(area/3.4)), sqrt(area/3.2), Scalar(0,255,0), -1, 8, 0 );
					std::cout<<area;
				}
            }
        }
    }
    printf("number is %d\n", num);
	destroyWindow("Canny");
	cvDestroyWindow("Tool");
    cvShowImage("Dice",&(IplImage)dice);
	/*
	IplImage *src=cvLoadImage("dice.jpg", 1);
    Mat input = Mat(src);
	IplImage* imgHSV = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
    cvCvtColor(src, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
	IplImage* imgThresh = GetThresholdedImage(imgHSV);
	

	imshow("input", input);
    waitKey();

    Mat redOnly = Mat(imgThresh);

    imshow("redOnly", redOnly);
    waitKey();

	*/


    waitKey();

    return 0;
}