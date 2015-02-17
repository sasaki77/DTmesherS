#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include "point.h"

using namespace std;

class Triangle {
    private:
        Node   *p[3];
        bool   isExist;
    
    public:
        Triangle(){
            for(int i=0;i<3;i++){
                p[i]     = NULL;
                //p2[i]    = NULL;
                //nei[i]   = NULL;
                isExist = false;
            }
        }

        void  setNodes( Node*, Node*, Node* );
        void  setExist( bool );
        Node* getNode( unsigned int );
        bool  getExist();
};

#endif /* end of include guard */
