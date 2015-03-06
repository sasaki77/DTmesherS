#ifndef ADD_NODE_OBLATENESS
#define ADD_NODE_OBLATENESS

#include "add_node_interface.h"

class AddNodeOblateness :public AddNodeInterface{
	public:
		AddNodeOblateness(); 
		virtual ~AddNodeOblateness() {};
		virtual Node* getAdditionalNode();
	private:
		vector< Triangle* > oblOrderTriangles;
		double min_area;     // 最小面積 これ以下の面積の要素は分割しない
};

#endif /* end of include guard */
