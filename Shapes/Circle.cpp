#include "Circle.h"
#include "Shape.h"
#include "Point.h"

Circle::Circle(int x, int y, float r) { //Set the leftTop, radius and calculate the points.
	leftTop->setPoint(x, y);
	radius = r;
	calculatePoints(x, y, r);
}

void Circle::calculatePoints(int x, int y, float r) { //Calculate the remaining point.
	Point* rightBottom = new Point;
	rightBottom->setPoint(x + 2 * r, y + 2 * r);
	points.push_back(leftTop);
	points.push_back(rightBottom);
}

void Circle::move(int newX, int newY) { //Clear points, set the new one, recalculate the rightBottom
	points.clear();
	leftTop->setPoint(newX, newY);
	calculatePoints(newX, newY, radius);
}

void Circle::calculateArea(float radius) { //Calculate and output area.
	float area = (radius * radius) * 3.14159;
	cout << "Area=" << area << endl;
}

void Circle::calculatePerimeter(float radius){ //Calculate and output perimeter.
	float perimeter = 2 * 3.14 * radius;
	cout << "Perimeter=" << perimeter << endl;
}

void Circle::scale(float scaleX, float scaleY) { //Check to see if ScaleX == ScaleY, scale and recalculate the points.
	if (scaleX != scaleY) {
		cout << "The two scale values must be the same!" << endl;
	}
	else {
		float x = leftTop->getX();
		float y = leftTop->getY();
		points.clear();
		radius = scaleX * radius;
		calculatePoints(x, y, radius);
	}
}

void Circle::toString() {
	cout << "Circle [r=" << radius << "]" << endl;
	cout << "Point[";
	for (int i = 0; i < points.size(); i++) {
		int x = points[i]->getX();
		int y = points[i]->getY();
		cout << "(" << x << "," << y << ")";
	}
	cout << "]" << endl;
	calculateArea(radius);
	calculatePerimeter(radius);
	cout << endl;
};