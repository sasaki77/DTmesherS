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

		bool startDT();
		void setInputFile(string _infile){ 
			infile_name = _infile;
		};

	private:
		Point  max_point, min_point;
        double max_distance;
		string infile_name;
		string outfile_name;
		int form;

		vector< Node* >     nodes;
        vector< Edge* >     edges;
        vector< Triangle* > triangles;

		bool inputParam();
        void nodesNormalize();      // nodesNormalize()で全ての節点を0<=x<=1,0<=y<=1に規格化する
        void nodesDenormalize();
        void setSuperTriangle();
};

#endif /* end of include guard */
