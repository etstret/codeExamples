#pragma once
#include <iostream>

using namespace std;

class Point {
private:
	int x = 0;
	int y = 0;

public:
	Point() { x = 0; y = 0; } //Point default constructor
	void setPoint(int xval, int yval) { //Point setter
		x = xval;
		y = yval;
	}
	int getX() { return x; }; //Point getter
	int getY() { return y; };
};