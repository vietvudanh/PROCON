#include"Dice.h"
#include"opencv2/core/core.hpp"

#include<math.h>
#include<iostream>

using namespace std;
using namespace cv;

#define DIFF 20
#define PI 3.14159265
#define ANG1 25.00
#define ANG2 65.00

//Default constructor
Dice::Dice(){}

//Override Constructor
Dice::Dice(vector<Point> position, int value ){
	// points
	this->position = vector<Point>(position);
	this->value = value;
	//center
	center.x = (position[0].x + position[1].x + position[2].x + position[3].x)/4;
	center.y = (position[0].y + position[1].y + position[2].y + position[3].y)/4;
	//edge
	for( int i = 0; i < position.size() ; i++){
		double temp = norm(position[i] - position[(i+1)%4]);
		edge.push_back(temp);
	}
	// min, max
	maximum = minimum = edge[0];
	for(int i =1; i < edge.size(); i++){
		if(edge[i] < minimum){
			minimum = edge[i];
		}
		if(edge[i] > maximum){
			maximum = edge[i];
		}
	}
	//angle
	angle = 90.0;
	for(int i = 0; i < position.size(); i++){
		double temp = atan2( (position[i].y-position[(i+1)%4].y), (position[i].x-position[(i+1)%4].x));
		temp = abs(temp);
		temp = temp/PI * 180;
		if(temp > 90.0){
			temp = 180 - temp;
		}
		if(temp <= angle){
			angle = temp;
		}
	}
}

//constructor
Dice::Dice(Point center, bool rotate, int minimum){
	//position
	this->position.push_back( Point(center.x - int(minimum*0.75),center.y - int(minimum*0.75)) );
	this->position.push_back( Point(center.x + int(minimum*0.75),center.y - int(minimum*0.75)) );
	this->position.push_back( Point(center.x - int(minimum*0.75),center.y + int(minimum*0.75)) );
	this->position.push_back( Point(center.x + int(minimum*0.75),center.y + int(minimum*0.75)) );

	//center
	this->center = Point(center);

	//angle
	if(rotate)
		this->angle = 45;
	else
		this->angle = 0;
}

//Check if it is really a dice
bool Dice::isDice(){

	for(int i = 0; i < edge.size(); i++){
		double temp = edge[i] - edge[(i+1)%4];
		if (temp > 10){
			return false;
		}
	}
	return true;
}

//is rotate
bool Dice::isRotate(){
	if ( ANG1<= angle && angle <= ANG2)
		return true;
	return false;
}

//print
void Dice::print(){
	cout << angle << " ";
	cout << center << "--";
	for(int i =0; i < position.size(); i++){
		cout << position[i];
	}
	cout << endl;
}

//compare <
bool Dice::operator<(const Dice& dice2){
	int temp = abs(this->center.y - dice2.center.y);
	if( temp < DIFF){
		return this->center.x < dice2.center.x;
	}
	else{
		return this->center.y < dice2.center.y;
	}
}