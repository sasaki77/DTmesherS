#include <iostream>
#include <cfloat>
#include "DT.h"

using namespace std;

bool DT::startDT() 
{
	cout << "start DT" << endl;
	inputParam();
	cout << "form = " << form << endl;

	for (int i = 0; i < nodes.size(); i++) {
		cout << i << ":(" << nodes[i]->getX() << "," << nodes[i]->getY() << ")" << endl;
	}

	for (int i = 0; i < edges.size(); i++) {
		cout << i << ":length = "  << edges[i]->getLength() << ", bc = " << edges[i]->bc.getBC() << "," << edges[i]->bc.getVal()  << endl;
		cout << "material = " << edges[i]->material.getMaterial() << endl;
	}

    nodesNormalize();

	for (int i = 0; i < nodes.size(); i++) {
		cout << i << ":(" << nodes[i]->getX() << "," << nodes[i]->getY() << ")" << endl;
	}

    nodesDenormalize();

	for (int i = 0; i < nodes.size(); i++) {
		cout << i << ":(" << nodes[i]->getX() << "," << nodes[i]->getY() << ")" << endl;
	}

    setSuperTriangle();
    for (int i=0; i < triangles.size(); i++) {
        for( int j=0; j<3; j++ ){
            Node* p = triangles[i]->getNode(j);
            cout << "p" << j << " = (" << p->getNum() << "," << p->getX() << "," << p->getY() << ")" << endl;
        }
        cout << "exist: " << triangles[i]->getExist() << endl;
    }

    outputResult();

	return true;
}


void DT::nodesNormalize()
{
    cout << "start normalize" << endl;

    max_point.set( -DBL_MAX, -DBL_MAX );
    min_point.set( DBL_MAX, DBL_MAX );

    for( unsigned int i=0; i<nodes.size(); i++ ){
        double x = nodes[i]->getX();
        double y = nodes[i]->getY();
        if( min_point.getX() > x ) min_point.setX( x );
        if( min_point.getY() > y ) min_point.setY( y );
        if( max_point.getX() < x ) max_point.setX( x );
        if( max_point.getY() < y ) max_point.setY( y );
    }

    double x_dis = max_point.getX() - min_point.getX();
    double y_dis = max_point.getY() - min_point.getY();
    max_distance = (x_dis > y_dis) ? x_dis:y_dis ;

    for( unsigned int i=0; i<nodes.size(); i++ ){
        nodes[i]->setX( ( nodes[i]->getX() - min_point.getX() ) / max_distance );
        nodes[i]->setY( ( nodes[i]->getY() - min_point.getY() ) / max_distance );
    }

    //for(unsigned int i=0; i<edge.size(); i++){
    //    if( typeid(*edge[i]) == typeid(CircleEdge) ){
    //        CircleEdge *c       = dynamic_cast<CircleEdge*>(edge[i]);
    //        c->center.x  = (c->center.x-minx) / square;
    //        c->center.y  = (c->center.y-miny) / square;
    //        c->r        /= square;
    //    }
    //}
}

void DT::nodesDenormalize()
{
    cout << "start denormalize" << endl;
    for( unsigned int i=0; i<nodes.size(); i++ ){
        nodes[i]->setX( nodes[i]->getX() * max_distance + min_point.getX() );
        nodes[i]->setY( nodes[i]->getY() * max_distance + min_point.getY() );
    }

    //for(unsigned int i=0; i<edge.size(); i++){
    //    if( typeid(*edge[i]) == typeid(CircleEdge) ){
    //        CircleEdge *c       = dynamic_cast<CircleEdge*>(edge[i]);
    //        c->center.x  = c->center.x*square + minx;
    //        c->center.y  = c->center.y*square + miny;
    //        c->r        *= square;
    //    }
    //}
}

void DT::setSuperTriangle()
{
    SuperNode *p1 = new SuperNode( -1.23, -0.50 );
    SuperNode *p2 = new SuperNode(  2.23, -0.50 );
    SuperNode *p3 = new SuperNode(  0.50,  2.50 );

    nodes.push_back( p1 );
    nodes.push_back( p2 );
    nodes.push_back( p3 );

    Triangle *t = new Triangle();
    t->setNodes( p1, p2, p3 );
    t->setExist( true );
    triangles.push_back( t );
}
