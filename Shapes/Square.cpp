#include "Shape.h"
#include "Square.h"
#include "Point.h"

Square::Square(int x, int y, float e) {
	leftTop->setPoint(x, y); //Set the left point for later calculation
	edge = e; //Set the edge
	calculatePoints(x, y, e); //Calculate points based on parameter input
}

void Square::calculatePoints(int x, int y, float edge) {
	Point* rightTop = new Point; //Create the other points
	Point* rightBottom = new Point;
	Point* leftBottom = new Point;

	rightTop->setPoint(x + edge, y); //Calculate the remaining points
	rightBottom->setPoint(x + edge, y + edge); 
	leftBottom->setPoint(x, y + edge);

	points.push_back(leftTop); //Push the points into the point vector, ordered.
	points.push_back(rightTop);
	points.push_back(rightBottom);
	points.push_back(leftBottom);
}

void Square::move(int newX, int newY) {
	points.clear(); //Clear the points vector to allow for new entries.
	leftTop->setPoint(newX, newY); //Calculate the new leftTop based on the x,y parameters of the move command.
	points.push_back(leftTop); //Push back the new leftTop and recalculate the new points.
	calculatePoints(newX, newY, edge);
}

void Square::calculateArea(float edge) {
	float area = edge * edge; //Calculate and output the area.
	cout << "Area: " << area << endl;
}

void Square::calculatePerimeter(float edge) {
	float perimeter = edge * 4; //Calculate the perimeter.
	cout << "Perimeter=" << perimeter << endl;
}

void Square::scale(float scaleX, float scaleY) {
	if (scaleX != scaleY) { //As scaleX can't be different to scaleY, and vice-versa, to preserve the square's integrity, we check to see if they're different.
		cout << "The two scale values must be the same!" << endl;
	}
	else {
		float x = leftTop->getX(); //Get the X and Y of the leftTop.
		float y = leftTop->getY();
		points.clear(); //Clear the remaining points.
		edge = scaleX * edge;
		calculatePoints(x, y, edge); //Calculate the new ones based on the scale factor.
	}
}

void Square::toString() {
	cout << "Square [e=" << edge << "]" << endl;
	cout << "Point[";
	for (int i = 0; i < points.size(); i++) { //Iterate through the points vector to output it.
		int x = points[i]->getX();
		int y = points[i]->getY();
		cout << "(" << x << "," << y << ")";
	}
	cout << "]" << endl;
	calculateArea(edge);
	calculatePerimeter(edge);
	cout << endl;
};