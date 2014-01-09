/*
	Class Dice represent a dice. A dice has 3 main attributes: 
		position(4 angle of a square)
		rotate: true or false
		value: 1-6
*/

#include<vector>
#include"opencv2/core/core.hpp"

using namespace cv;
using namespace std;

#ifndef DICE

class Dice{
	private:
		vector<Point> position;
		vector<double> edge;
		int size;
		int value;
		double angle;
		Point center;
		double minimum;
		double maximum;
		
	public:
		Dice();
		Dice(vector<Point> position, int value = 0);
		Dice(Point center, bool rotate, int minimum);
		inline int getSize(){return size;}
		inline int getValue(){return value;}
		inline vector<Point> getPosition(){return position;}
		double getMinimum(){return minimum;}
		double getMaximum(){return maximum;}
		inline Point getCenter(){return center;};
		bool isRotate();
		bool isDice();
		void print();
		bool operator < (const Dice& dice2);
		bool operator > (const Dice& dice2);
};

#endif DICE