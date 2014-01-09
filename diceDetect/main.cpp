// The "Square Detector" program.
// It loads several images sequentially and tries to find squares in
// each image

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Dice.h"

#include <iostream>
#include <math.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <windows.h>

using namespace cv;
using namespace std;

//Define constants
double minimum = 0;			//minimum edge of a squares
int lthresh = 1000, hthresh = 1000, N = 10;	//detect: threshold cor Canny, number of tries

//list of dices
vector<Dice> dices;

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();

    Mat pyr, timg, gray0(image.size(), CV_8U), gray;

    // down-scale and upscale the image to filter out the noise
    pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    pyrUp(pyr, timg, image.size());
    vector<vector<Point> > contours;

    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 100 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
				
                Canny(gray0, gray, lthresh, hthresh, 5);

                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));

			}
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0name
                gray = gray0 >= (l+1)*255/N;
            }


            // find contours and store them all as a list
            findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

            vector<Point> approx;

            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.035, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 300 &&
                    isContourConvex(Mat(approx)) )
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
            }
        }
    }
}

// the function draws all the squares in the image
static void drawSquares( Mat& image )
{
    for( size_t i = 0; i < dices.size(); i++ )
    {
		vector<Point> temp = dices[i].getPosition();
        const Point* p = &temp[0];
        int n = (int)temp.size();
		if(dices[i].isRotate()){
			polylines(image, &p, &n, 1, true, Scalar(0,0,255), 2, CV_AA);
		}
		else
			polylines(image, &p, &n, 1, true, Scalar(0,255,0), 2, CV_AA);	
    }
}

//write to file
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
			ofile << temp.getPosition()[j].x << ", " << temp.getPosition()[j].y << ", ";
		}
		ofile << temp.isRotate() << endl;
	}
	ofile.close();
}

//filter: Remove too large or duplicate squares
void filter(const vector<vector<Point>>& squares){
	
	//initiate minimum and dices
	Dice temp = Dice(squares[0]);
	minimum = temp.getMinimum();
	dices.push_back(temp);
	for(int i =1; i < squares.size(); i++){
		temp = Dice(squares[i]);
		if(temp.getMinimum() < minimum){
			minimum = temp.getMinimum();
		}
		if(temp.isDice()){
			dices.push_back(temp);
		}
	}
		
	//get rid of too large square
	for(int i =0; i < dices.size(); i++){
		if( dices[i].getMinimum() > 3*minimum || dices[i].getMaximum() > 3*minimum){
			dices.erase(dices.begin() + i);
		}
	}
		
	//get rid of duplicate square
	for(int i = 0; i < dices.size(); i++){
		for(int j = i+1; j < dices.size(); j++){
			Point p1 = dices[i].getCenter();
			Point p2 = dices[j].getCenter();
			double distance = norm(p1 - p2);
			if (distance < minimum){
				dices.erase(dices.begin() + j);
				j--;
			}
		}
	}
	
}

//bubble sort
void BubbleSort()
{
    int i, j, flag = 1;    // set flag to 1 to start first pass
    Dice temp;             // holding variable
    int dicesSize = dices.size( ); 
    for(i = 1; (i <= dicesSize) && flag; i++)
	{
        flag = 0;
        for (j=0; j < (dicesSize -1); j++)
        {
            if (dices[j+1] < dices[j])      // ascending order simply changes to <
            {
                temp = dices[j];			// swap elements
                dices[j] = dices[j+1];
                dices[j+1] = temp;
                flag = 1;					// indicates that a swap occurred.
            }
        }
     }
     return;   //arrays are passed to functions by address; nothing is returned
}

//sort dices
void sort(){
	BubbleSort();
}

//draw a box
void draw_box( Mat& image, Point center, bool rotate ){
	if(rotate){
		RotatedRect rRect(center, Size(1.5*minimum,1.5*minimum), 45);
		Point2f vertices[4];
		rRect.points(vertices);
		for (int i = 0; i < 4; i++)
			line(image, vertices[i], vertices[(i+1)%4], Scalar(255,0,0), 2);
		dices.push_back(Dice(center,true, minimum));
	}
	else{
		Rect rect(Point(center.x - int(minimum*0.75),center.y - int(minimum*0.75)),Point(center.x + int(minimum*0.75),center.y + int(minimum*0.75)) );
		rectangle(image, rect, Scalar(255,255,255),2);
		dices.push_back(Dice(center, false, minimum));
	}
}

// Implement mouse callback
void my_mouse_callback( int event, int x, int y, int flags, void* param ){
	Mat image;
	image = *((Mat*)param);

	switch( event ){
		case CV_EVENT_MOUSEMOVE:
			break;
		case CV_EVENT_LBUTTONDOWN:
			draw_box(image, Point(x,y), false);
			break;

		case CV_EVENT_RBUTTONDOWN:
			draw_box(image, Point(x,y), true);
			break;
	}
}
//showGUI
void showGUI(Mat& image){
	const char* name = "fill";

	Mat temp;
	temp = image.clone();
	
	cvNamedWindow( name );
	// Set up the callback
	cvSetMouseCallback( name, my_mouse_callback, &image);

	//loop
	while(1){
		imshow(name,image);
		if( cvWaitKey( 15 )==27 ) 
			break;
	}
	destroyWindow( name );

}

//main funtcion
int main(int argc, char** argv)
{
	char * name = "src.jpg";

	//list of detected squares
    vector<vector<Point> > squares;

	//create an instance of the image
    Mat image = imread(name, 1);
    if( image.empty() )
    {
        cout << "Couldn't load " << name << endl;
		exit(1);
    }

	cout << "finding.." << endl;
	//detect squares
    findSquares(image, squares);

	//store squares to dices
	filter( squares);
	squares.empty();
	
	cout << "drawing.." << endl;
	//draw squares to image
    drawSquares(image);
	
	cout << "showing to user for filling..." << endl;
	showGUI(image);

	cout << "sorting.." << endl;
	//sort
	sort();

	int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;	//draw font
	double fontScale = 0.4;		//draw font
	int thickness = 1;			//draw font
	for(int i =0; i < dices.size(); i++){
		//center
		ostringstream conv;
		conv << i;
		string text = conv.str();
		putText(image, text, dices[i].getCenter(), fontFace, fontScale, Scalar(255,0,0), thickness );
	}
	//create a window
	char* wndname = "Last check";
    namedWindow( wndname, 1 );
	imshow(wndname, image);
	//wait for next pictures
    int c = waitKey();
	
	cout << "storing to jpg.." << endl;
	//write image
	imwrite("image.jpg", image);

	cout << "storing to txt.." << endl;
	//write dices list to file
	char* wname = "dices.txt";
	write(wname);

	cout << "Done.." << endl;
    return 0;
}