#include <iostream>
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

    neighbor_triangle[0] = nei;
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
