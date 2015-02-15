#include <iostream>
#include "DT.h"

using namespace std;

bool DT::startDT() {
	cout << "start DT" << endl;
	inputParam();
	cout << "form = " << form << endl;

	for (int i = 0; i < nodes.size(); i++) {
		cout << i << ":(" << nodes[i]->getX() << "," << nodes[i]->getY() << ")" << endl;
	}

	for (int i = 0; i < edges.size(); i++) {
		cout << i << ":length = "  << edges[i]->getLength() << ", bc = " << edges[i]->getBC() << "," << edges[i]->getBCVal()  << endl;
	}

	return true;
}
