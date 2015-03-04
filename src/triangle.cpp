#include <iostream>
#include <cfloat>
#include "edge.h"
#include "triangle.h"

using namespace std;

void Triangle::setNodes( Node* p0, Node* p1, Node* p2 )
{
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
}

void Triangle::setExist( bool _isExist ) 
{
    isExist = _isExist;    
}

void  Triangle::setNeighborTri( Triangle* nei1, Triangle* nei2, Triangle* nei3 )
{
    neighbor_triangle[0] = nei1;
    neighbor_triangle[1] = nei2;
    neighbor_triangle[2] = nei3;
}

void  Triangle::setNeighborTriOne( int index, Triangle* nei )
{
    if( index < 0 || index > 2 ){
        cout << "setNeighborTriOne() index must be from 0 to 2" << endl;
        return;
    }

    neighbor_triangle[index] = nei;
}

void Triangle::calcOblateness()
{
	StraightEdge e[3];
	for( int i=0 ;i<3; i++ ){
		e[i].setNodes( p[i], p[(i+1)%3] );
	}

	double a,b,c;
	a = e[0].getLength();
	b = e[1].getLength();
	c = e[2].getLength();

	double theta[3];
	theta[0] = acos( (b*b + c*c - a*a)/(2*b*c) );
	theta[1] = acos( (c*c + a*a - b*b)/(2*c*a) );
	theta[2] = acos( (a*a + b*b - c*c)/(2*a*b) );

	double max_theta = DBL_MIN;
	for( int i=0; i<3;i++ ){
		if( max_theta<theta[i] ){
			max_theta = theta[i];
		}
	}
	oblateness = max_theta;
}

Node* Triangle::getNode( unsigned int index )
{
    if( index < 0 || index > 2 ){
        cout << "getNode() index must be from 0 to 2" << endl;
        return NULL;
    }

    return p[ index ];
}

bool Triangle::getExist()
{
    return isExist;
}

Triangle* Triangle::getNeighborTri( int index )
{
    if( index < 0 || index > 2 ){
        cout << "getNeighborTri() index must be from 0 to 2" << endl;
        return NULL;
    }
    
    return neighbor_triangle[ index ];
}

double Triangle::getOblateness()
{
	return oblateness;
}
