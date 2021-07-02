#pragma once

#include "Shape.h"
#include "Movable.h"

class Square : public Shape, public Movable {
private:
	float edge;

public:
	virtual void calculatePoints(int x, int y, float e);
	Square(int x, int y, float e);
	void move(int newX, int newY);
	void calculateArea(float edge);
	void calculatePerimeter(float edge);
	void scale(float scaleX, float scaleY);
	void toString();
};