#include "Shape.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Square.h"
#include "Movable.h"

#include <iostream>
#include <string>

using namespace std;

int main()
{
	string userCommand;
	vector <Shape*> shapes;     // this one will hold shapes
	vector <string> parameters; // this one will hold parameters for the commands


	while (userCommand.compare("exit") != 0)
	{
		parameters.clear();
		cout << "Enter the command: ";

		getline(cin, userCommand);

		char* cstr = new char[userCommand.length() + 1];

		strcpy_s(cstr, userCommand.length() + 1, userCommand.c_str());

		// implement a string tokenizer to populate the parameters vector 
		// check function strtok_s
		char* ctx = NULL;
		char* savePtr = strtok_s(cstr, " ", &ctx);
		while (savePtr != NULL) {
			string strPtr(savePtr);
			parameters.push_back(strPtr);

			savePtr = strtok_s(NULL, " ", &ctx);
		}

		string command = parameters[0];



		if (command.compare("addR") == 0) {
			try {
				int x = stoi(parameters[1].c_str()); 
				int y = stoi(parameters[2].c_str());
				int h = stoi(parameters[3].c_str());
				int w = stoi(parameters[4].c_str());

				Rectangle* r = new Rectangle(x, y, h, w);
				shapes.push_back(r);
				r->toString(); /
			}
			catch (invalid_argument) {
				cout << "You must input an integer!" << endl;
			}
		}
		else if (command.compare("addS") == 0) {
			// get parameters
			// ...
			try {
				int x = stoi(parameters[1].c_str());
				int y = stoi(parameters[2].c_str());
				int e = stoi(parameters[3].c_str());
				Square* s = new Square(x, y, e);
				shapes.push_back(s);
				s->toString();
			}
			catch (invalid_argument) {
				cout << "You must input an integer!" << endl;
			}

		}

		if (command.compare("addC") == 0) {
			// get parameters
			// ...
			try {
				int x = stoi(parameters[1].c_str());
				int y = stoi(parameters[2].c_str());
				int r = stoi(parameters[3].c_str());
				Circle* c = new Circle(x, y, r);
				shapes.push_back(c);
				c->toString();
			}
			catch (invalid_argument) {
				cout << "You must input an integer!" << endl;
			}
		}
		else if (command.compare("scale") == 0) {
			try {
				int shapeNo = stoi(parameters[1].c_str());
				if (shapeNo > shapes.size()) {
					cout << "This index does not exist!" << endl;
					break;
				}
				float x = stoi(parameters[2].c_str());
				float y = stoi(parameters[3].c_str());

				Movable* m = dynamic_cast<Movable*>(shapes[shapeNo - 1]);
				m->scale(x, y);
				shapes[shapeNo - 1]->toString();
			}
			catch (invalid_argument) {
				cout << "You must input an integer!" << endl;
			}

		}
		else if (command.compare("move") == 0) {
			// move object at index 
			int shapeNo; // read from parameters
			shapeNo = stoi(parameters[1].c_str());
			if (shapeNo > shapes.size()) {
				cout << "This index does not exist!" << endl;
				break;
			}
			int x = stoi(parameters[2].c_str());
			int y = stoi(parameters[3].c_str());
			Movable* m = dynamic_cast<Movable*>(shapes[shapeNo - 1]);
			m->move(x, y);

			shapes[shapeNo - 1]->toString();
		}
		else if (command.compare("display") == 0) {
			for (int i = 0; i != shapes.size(); i++) {
				shapes[i]->toString();
			}
		}

		cout << endl << endl;
	}

	cout << "Press any key to continue...";
	std::getchar();

	return 0;
}