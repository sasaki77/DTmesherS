#ifndef DT_H
#define DT_H

#include <string>
#include "point.h"

using namespace std;

class DT {
	public:
		DT(){
		}

		int startDT();
	private:
		Point max_point, min_point;
		string infile_name;
		string outfile_name;
};

#endif /* end of include guard */
