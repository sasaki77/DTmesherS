#ifndef DT_H
#define DT_H

#include <string>
#include <vector>

#include "point.h"
#include "edge.h"
#include "triangle.h"

using namespace std;

class DT {
	public:
		DT(){
		}

        ~DT(){
            for( unsigned int i=0; i<nodes.size(); i++ ){
                if(nodes[i] != NULL) delete nodes[i];
            }
            for( unsigned int i=0; i<triangles.size(); i++ ){
                if(triangles[i] != NULL) delete triangles[i];
            }
            for( unsigned int i=0; i<edges.size(); i++ ){
                if(edges[i] != NULL) delete edges[i];
            }
            //for( unsigned int i=0; i<bnd.size(); i++ ){
            //    if(bnd[i] != NULL) delete bnd[i];
            //}
        }

		bool startDT();

		void setInputFile(string _infile){ 
			infile_name = _infile;
		};

		void setOutputFile(string _outfile){ 
			outfile_name = _outfile;
		};

	private:
		Point  max_point, min_point;
        double max_distance;
		string infile_name;
		string outfile_name;
		int    form;

		vector< Node* >     nodes;
        vector< Edge* >     edges;
        vector< Triangle* > triangles;

		bool inputParam();
		bool outputResult();

        void nodesNormalize();      // nodesNormalize()で全ての節点を0<=x<=1,0<=y<=1に規格化する
        void nodesDenormalize();
        void setSuperTriangle();
        void removeIllegalTriangles();
};

#endif /* end of include guard */
