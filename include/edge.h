#ifndef EDGE_H
#define EDGE_H

#include <cmath>
#include "point.h"
#include "bc.h"
#include "material.h"

class Edge
{
	protected:
		Node *p[2];

	public:
        BoundaryCondition bc;
        Material          material;

		Edge(){
			p[0] = NULL;
			p[1] = NULL;
		}

		Edge( Node *_p0, Node *_p1 ){
			p[0] = _p0;
			p[1] = _p1;
		}

        virtual ~Edge () {}

		void setNodes( Node *_p0, Node *_p1 ){
			p[0] = _p0;
			p[1] = _p1;
		}

		Node* getNode( int index ){
			if( index < 0 || index > 1 ){
				return NULL;
			}
			return p[index];
		}

		virtual double getLength() = 0;
		//virtual Point  divPoint(int n,int i) = 0;

		bool isEqual( const Edge& _edge ){
			return ( (p[0]->isEqual(*(_edge.p[0])) && p[1]->isEqual( *(_edge.p[1]) ) ) || 
					 (p[0]->isEqual(*(_edge.p[1])) && p[1]->isEqual( *(_edge.p[0]) ) ) );
		}
};

class StraightEdge: public Edge{
	public:
		StraightEdge():Edge(){}

		StraightEdge( Node *_p0, Node *_p1 ):Edge(_p0,_p1){}

        virtual ~StraightEdge () {}

		double getLength(){
			double dx = fabs(p[1]->getX() - p[0]->getX() );
			double dy = fabs(p[1]->getY() - p[0]->getY() );
			return sqrt( dx*dx + dy*dy );
		}

		//Point divPoint(int n,int i){
		//  if(i>n || i==0) cerr << "error divPoint()" << endl;
		//  Point retP;
		//  retP.x = ( (n-i+1) * p[0]->x + i * p[1]->x) / (n+1);
		//  retP.y = ( (n-i+1) * p[0]->y + i * p[1]->y) / (n+1);
		//  return retP;
		//}
};

#endif /* end of include guard */
