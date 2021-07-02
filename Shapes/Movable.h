#pragma once
#include "Shape.h"

class Movable {
public:
	virtual void move(int newX, int newY);
	virtual void scale(float scaleX, float scaleY);
};