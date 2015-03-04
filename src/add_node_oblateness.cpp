#include <vector>

#include "DT.h"
#include "add_node_oblateness.h"

Node* AddNodeOblateness::getAdditionalNode()
{
	const vector< Triangle* >& triangles = DT::getTriangles();
	return (new Node(1,1) );
}
