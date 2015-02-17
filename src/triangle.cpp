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
