#include <iostream>
#include <string>
#include "DT.h"

using namespace std;

int main(int argc, char const* argv[])
{
	DT dt;
	dt.setInputFile(  (string) argv[1] );
	dt.setOutputFile( (string) argv[1] );
	dt.startDT();
	
	return 0;
}
