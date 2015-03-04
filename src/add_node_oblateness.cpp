#include <iostream>
#include <vector>

#include "DT.h"
#include "add_node_oblateness.h"

using namespace std;

static bool compareOblateness( Triangle* , Triangle* );

AddNodeOblateness::AddNodeOblateness(){
	const vector< Triangle* >& triangles = DT::getTriangles();
	for( unsigned int i=0; i<triangles.size(); i++ ){
		oblOrderTriangles.push_back( triangles[i] );
	}
}

Node* AddNodeOblateness::getAdditionalNode()
{
	const vector< Triangle* >& triangles = DT::getTriangles();
	sort( oblOrderTriangles.begin(), oblOrderTriangles.end(), compareOblateness );
	return (new Node(1,1) );
}

static bool compareOblateness( Triangle* left, Triangle* right ){
	left->calcOblateness();
	right->calcOblateness();
	double left_obl  = left->getOblateness();
	double right_obl = right->getOblateness();

	return ( left_obl > right_obl );
}
