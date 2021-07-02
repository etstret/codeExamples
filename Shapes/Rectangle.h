#pragma once

#include "Shape.h"
#include "Movable.h"

class Rectangle : public Shape, public Movable {
private:
	float w;
	float h;

public:
	virtual void calculatePoints(int x, int y, float h, float w);
	void calculateArea(float h, float w);
	void calculatePerimeter(float h, float w);
	Rectangle(int x, int y, float h, float w);
	void move(int newX, int newY);
	void scale(float scaleX, float scaleY);
	void toString();
};