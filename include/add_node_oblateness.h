#ifndef ADD_NODE_OBLATENESS
#define ADD_NODE_OBLATENESS

#include "add_node_interface.h"

class AddNodeOblateness :public AddNodeInterface{
	public:
		AddNodeOblateness() {};
		virtual ~AddNodeOblateness() {};
		virtual Node* getAdditionalNode();
};

#endif /* end of include guard */
