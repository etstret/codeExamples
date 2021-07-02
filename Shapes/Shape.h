#pragma once
#include "Point.h"
#include <vector>
class Shape {
private:
	bool isCircular;
	float area;
	float perimeter;
protected:
	Point* leftTop = new Point; //leftTop setter
	std::vector<Point*> points;
public:
	virtual void calculatePoints();
	virtual void calculateArea(float h, float w);
	virtual void calculateArea(float radEdge);
	virtual void calculatePerimeter(float h, float w);
	virtual void calculatePerimeter(float radEdge);
	virtual void toString();
};