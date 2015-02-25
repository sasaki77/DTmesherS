#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include "point.h"

using namespace std;

class Triangle {
    private:
        Node     *p[3];
        bool     isExist;
        Triangle *neighbor_triangle[3];
    
    public:
        Triangle(){
            for(int i=0;i<3;i++){
                p[i] = NULL;
                //p2[i]    = NULL;
                neighbor_triangle[i] = NULL;
                isExist = false;
            }
        }

        void      setNodes( Node*, Node*, Node* );
        void      setExist( bool );
        void      setNeighborTri( Triangle*, Triangle*, Triangle* );
        void      setNeighborTriOne( int, Triangle* );
        Node*     getNode( unsigned int );
        bool      getExist();
        Triangle* getNeighborTri( int );
};

#endif /* end of include guard */
