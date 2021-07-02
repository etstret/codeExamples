#include "Shape.h"
#include "Rectangle.h"
#include "Point.h"
#include "Movable.h"
#include <iostream>
using namespace std;

Rectangle::Rectangle(int x, int y, float height, float width) {
	leftTop->setPoint(x, y); //Set the left point for later calculation
	h = height; //Set the height and width based on parameters.
	w = width;
	calculatePoints(x, y, h, w); //Calculate points based on parameter input
}

void Rectangle::calculatePoints(int x, int y, float height, float width) { //Create the other points, assign them their co-ordinates and push them into the vector.
	Point* rightTop = new Point;
	Point* leftBottom = new Point;
	Point* rightBottom = new Point;
	rightTop->setPoint(x + width, y);
	leftBottom->setPoint(x, y + height);
	rightBottom->setPoint(x + width, y + height);
	
	points.push_back(leftTop);
	points.push_back(rightTop);
	points.push_back(rightBottom);
	points.push_back(leftBottom);

}

void Rectangle::calculateArea(float h, float w) { //Calculate and output the area.
	float area = h * w;
	cout << "Area=" << area << endl;
}

void Rectangle::calculatePerimeter(float h, float w) { //Calculate and output the perimeter.
	float perimeter = (h * 2) + (w * 2);
	cout << "Perimeter=" << perimeter << endl;
}

void Rectangle::move(int newX, int newY) { //Clear the points, set the new leftTop and calculate the remaining points.
	points.clear();
	leftTop->setPoint(newX, newY);
	calculatePoints(newX, newY, h, w);
}

void Rectangle::scale(float scaleX, float scaleY) { //Get the old X and Y, clear the points vector, apply the scale factor to the height and width and recalculate the points.
	float x = leftTop->getX();
	float y = leftTop->getY();
	points.clear();
	h = scaleY * h;
	w = scaleX * w;
	calculatePoints(x, y, h, w);
	calculateArea(h, w);
}

void Rectangle::toString() {
	cout << "Rectangle[h=" << h << ",w=" << w << "]" << endl;
	cout << "Point[";
	for (int i = 0; i < points.size(); i++) {
		int x = points[i]->getX();
		int y = points[i]->getY();
		cout << "(" << x << "," << y << ")";
	}
	cout << endl;
	calculateArea(h, w);
	calculatePerimeter(h, w);
	cout << endl;
};