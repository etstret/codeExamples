#pragma once

#include "Shape.h"
#include "Movable.h"

class Circle : public Shape, public Movable {
private:
	float radius;

public:
	virtual void calculatePoints(int x, int y, float radius);
	Circle(int x, int y, float radius);
	void move(int newX, int newY);
	void calculateArea(float radius);
	void calculatePerimeter(float radius);
	void scale(float scaleX, float scaleY);
	void toString();
};

