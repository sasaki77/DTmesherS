#include <iostream>
#include <vector>
#include <algorithm>

#include "DT.h"
#include "add_node_oblateness.h"

using namespace std;

static bool compareOblateness( Triangle* , Triangle* );

AddNodeOblateness::AddNodeOblateness(){
	min_area = 1;

	const vector< Triangle* >& triangles = DT::getTriangles();
	for( unsigned int i=0; i<triangles.size(); i++ ){
		oblOrderTriangles.push_back( triangles[i] );
	}
}

Node* AddNodeOblateness::getAdditionalNode()
{
	const vector< Triangle* >& triangles = DT::getTriangles();

	// 偏平率で三角形をソートしておく
	sort( oblOrderTriangles.begin(), oblOrderTriangles.end(), compareOblateness );

	// 予め全三角形の面積を計算しておく
	for( unsigned int i=0; i<oblOrderTriangles.size(); i++ ){
		oblOrderTriangles[i]->calcArea();
	}

	// 新しい節点を置く三角形tを取得
	bool can_set = false;
	Triangle *t;
	while( !can_set ){
		min_area *= 0.95;
		for( unsigned int i=0; i<oblOrderTriangles.size(); i++ ){
			if( oblOrderTriangles[i]->getExist() && 
				oblOrderTriangles[i]->getArea() > min_area ){
				can_set = true;
				t = oblOrderTriangles[i];
			}
		}
	}
	
	// 最長辺eをStraightEdgeとして取得
	StraightEdge e( t->getNode(0), t->getNode(1) );
	StraightEdge temp_e;
	for( int i=1; i<3; i++ ){
		temp_e.setNodes( t->getNode(i), t->getNode((i+1)%3) );
		if( e.getLength() < temp_e.getLength() ){
			e = temp_e;
		}
	}

    Node *new_node = new Node( (e.getNode(0)->getX() + e.getNode(1)->getX())/2.0,
			       			   (e.getNode(0)->getY() + e.getNode(1)->getY())/2.0 );

	return new_node;
}

static bool compareOblateness( Triangle* left, Triangle* right ){
	left->calcOblateness();
	right->calcOblateness();
	double left_obl  = left->getOblateness();
	double right_obl = right->getOblateness();

	return ( left_obl > right_obl );
}
