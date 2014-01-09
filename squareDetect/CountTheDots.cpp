// CountTheDots.cpp : Count the dots in each dices
// Written by Thanh Soi :->

#include "stdafx.h"
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <windows.h>
#include "dice.h"
 
using namespace cv;
//Global variable
int p1, p2, a1,a2,cal;	//Value on trackbar: p1, p2 for threshold; a1, a2 for dot area; cal for calculation command
Mat dice, canny;		//Source Image and Canny version of it
IplImage *sample;		//Show sample area for dot filtering
Vector<Point> dots;		//Set of dots
Vector<Dice> dices;		//Set of dices

//Functions for I/O
void read(char* filename);								//read Data from Viet's file (dice's shape)
void write(char* filename);								//write Data for manual checking
void loadImage(const char* name);						//load Image and raw process
void initialize();										//initialize global variable
void showResult();										//show awesome result!!

//Functions for correction
void checkCanny();
void previewDots();

//Functions for counting the dots
bool onTwoSides(Point A, Point B, Point M, Point N);	// Decide if M and N on two sizes of AB segment
bool inside(Point p, Dice dice);						// Decide if p is belong to dice
void countTheDots();									// as its name :lol:





int main(int argc, const char** argv)
{
	read("dices.txt");
	loadImage("src.jpg");
	initialize();    
	//Cac bien kiem tra su thay doi cua p1 p2 a1 a2 cal (tren trackbar)
	int p1check=p1,p2check=p2,a1check=a1,a2check=a2,calcheck=1;	
	//Backup
	Mat canny0=canny.clone();
	Mat dice0=dice.clone();
	cout<<"Correction..."<<endl;
    while (true){		
		if (p1!=p1check||p2!=p2check||a1!=a1check||a2!=a2check){
			//Restore
			dice = dice0.clone();
			canny=canny0.clone();
			p1check=p1;p2check=p2;a1check=a1;a2check=a2;
			checkCanny();			
		}
		if (cal!=calcheck){
			//Restore
			dice = dice0.clone();
			canny=canny0.clone();
			calcheck=cal;				
			previewDots();
		}		
		int c = cvWaitKey(10);
		//If 'ESC' is pressed, break the loop
		if((char)c==27 )	
			break;
	}		
	countTheDots();
	showResult();
	write("dices_full.txt");
    waitKey();
    return 0;
}

void checkCanny() {
    Canny(dice, canny, p1, p2);
	cvShowImage("Count the dots",&(IplImage)canny);
	sample=cvCreateImage(cvSize(4*sqrt(1000/3.2),4*sqrt(1000/3.2)),IPL_DEPTH_8U, 3);
	cvCircle( sample, Point(sqrt(a1/3.1),sqrt(a1/3.1)), sqrt(a1/3.1), Scalar(255,255,255), -1, 8, 0 );
	cvCircle( sample, Point(2*sqrt(a1/3.1)+sqrt(a2/3.2),sqrt(a2/3.2)), sqrt(a2/3.2), Scalar(255,255,255), -1, 8, 0 );
	cvShowImage("Sample",sample);

}

void read(char* filename){
	ifstream ifile(filename, ios::in);
	if ( ifile.fail() ){
		cout << "Can't open file" << endl;
		exit(1);
	}

	//read each dice
	int x1,y1,x2, y2,x3, y3,x4, y4,rotate;
	while (ifile>>x1>>y1>>x2>>y2>>x3>>y3>>x4>>y4>>rotate){	
		vector<Point> dicePosition;
		dicePosition.push_back(Point(x1,y1));
		dicePosition.push_back(Point(x2,y2));
		dicePosition.push_back(Point(x3,y3));
		dicePosition.push_back(Point(x4,y4));
		dices.push_back(Dice(dicePosition));		
	}	
	ifile.close();
}

void write(char* filename){
	ofstream ofile(filename, ios::out);
	if ( ofile.fail() ){
		cout << "Can't open file" << endl;
		exit(1);
	}
	//write each dice
	for(int i = 0 ; i <dices.size(); i++){
		//positions
		Dice temp = dices[i];
		for(int j = 0; j < temp.getPosition().size(); j++){
			ofile << temp.getPosition()[j].x << " " << temp.getPosition()[j].y << " ";
		}
		ofile <<temp.isRotate()<<" "<< temp.getValue() << endl;
	}
	ofile.close();
}

bool onTwoSides(Point A, Point B, Point M, Point N){
	if (A.x==B.x)
		return((M.x-A.x)*(N.x-A.x)<=0);
	if (A.y==B.y)
		return((M.y-A.y)*(N.y-A.y)<=0);
	double fAB_M=(double(M.x)-double(A.x))/(double(A.x)-double(B.x))-(double(M.y)-double(A.y))/(double(A.y)-double(B.y));
	
	double fAB_N=(double(N.x)-double(A.x))/(double(A.x)-double(B.x))-(double(N.y)-double(A.y))/(double(A.y)-double(B.y));
	
	return (fAB_M*fAB_N<=0);
}
bool inside(Point p, Dice dice){
	//Math equation
	//f(x,y) = (x-x1)/(x1-x2)-(y-y1)/(y1-y2)
	//f1(C).f1(D)<0 and f2(A).f2(B)<0

	int num=0;
	for (int i=0;i<4;i++){
		if (onTwoSides(Point(0,0),p,dice.getPosition()[i],dice.getPosition()[(i+1)%4])&&onTwoSides(dice.getPosition()[i],dice.getPosition()[(i+1)%4],Point(0,0),p))
			num++;
	}
	
	return (num%2!=0);
}

void loadImage(const char* name){
	cout<<"Loading Image..."<<endl;
	IplImage* src0=cvLoadImage(name, 1);
	IplImage* src = cvCreateImage(cvSize(800, 600), src0->depth, src0->nChannels);
	cout<<"Resizing..."<<endl;
	cvResize(src0, src, CV_INTER_NN);
	cvSmooth(src, src, CV_GAUSSIAN,3,3);
	dice = Mat(src);
	Mat pyr, timg;
	pyrDown(dice, pyr, Size(dice.cols/2, dice.rows/2));
    pyrUp(pyr, timg, dice.size());
}

void initialize(){
	p1 = 150; p2 = 200;
	a1 = 3;   a2 = 200;
	cvShowImage("Sample",sample);
    cvNamedWindow("Tool");
	cvNamedWindow("Sample");
	cvResizeWindow("Tool",400,300);
	cvCreateTrackbar("Low Thres","Tool",&p1,1000);
	cvCreateTrackbar("High Thres","Tool",&p2,1000);
	cvCreateTrackbar("Min area","Tool",&a1,100);
	cvCreateTrackbar("Max area","Tool",&a2,1000);
	cvCreateTrackbar("Solve","Tool",&cal,1);
	checkCanny();
}

void previewDots(){
	dots.clear();
	for(int y=0;y<canny.size().height;y++)
	{
		uchar *row = canny.ptr(y);
		for(int x=0;x<canny.size().width;x++)
		{
			if(row[x] <= 128)
			{
				int area = floodFill(canny, Point(x,y), CV_RGB(0,0,160));
				if(area>a1*0.8 && area < a2*1.2) {
					circle( dice, Point(x+sqrt(area/3.4),y+sqrt(area/3.4)), sqrt(area/3.2), Scalar(0,255,0), -1, 8, 0 );
					dots.push_back(Point(x,y));
				}
			}
		}
	}
	cvShowImage("Count the dots",&(IplImage)dice);
}

void showResult(){
	for (int i = 0; i<dices.size();i++){
		ostringstream conv;
		conv << dices[i].getValue();;
		string text0 = conv.str();
		const char* text =text0.c_str();
		int x=dices[i].getPosition()[0].x;
		int y=dices[i].getPosition()[0].y;
		circle( dice, Point(x+1,y-11), 10, Scalar(0,0,0), -1, 8, 0 );
		cvPutText(&(IplImage)dice,text,Point(x-5,y-5), &cvFont(1.3,1), cvScalar(255,255,255));
	}    
	cvDestroyWindow("Tool");
    cvShowImage("Count the dots",&(IplImage)dice);
}

void countTheDots(){
	cout<<"Calculating..."<<endl;
	for (int i = 0; i<dices.size();i++)
		for (int j=0;j<dots.size();j++){
			if (inside(dots[j],dices[i]))
				dices[i].addDot();
			if (dices[i].getValue()>=6)
				break;
		}
}