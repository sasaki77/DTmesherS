#ifndef DT_H
#define DT_H

#include <string>
#include <vector>

#include "point.h"
#include "edge.h"

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
		Point max_point, min_point;
		string infile_name;
		string outfile_name;
		int form;

		vector< Node* >     nodes;
        vector< Edge* >     edges;

		bool inputParam();
};

#endif /* end of include guard */
