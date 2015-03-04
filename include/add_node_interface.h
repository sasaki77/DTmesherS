#ifndef ADD_NODE_INTERFACE
#define ADD_NODE_INTERFACE

class AddNodeInterface {
	public:
		AddNodeInterface() {};
		virtual ~AddNodeInterface() {};
		virtual Node* getAdditionalNode() = 0;
};

#endif /* end of include guard */
