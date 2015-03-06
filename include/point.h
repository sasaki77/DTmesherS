#ifndef POINT_H
#define POINT_H

class Point
{
	private:
		double x,y;

	public:
		Point():x(0),y(0){}
		Point( double _x, double _y ):x(_x),y(_y){}

        virtual ~Point () {}

		void set(double _x, double _y){
			x = _x;
			y = _y;
		}

        void setX( double _x ){
            x = _x;
        }

        void setY( double _y ){
            y = _y;
        }

		double getX(){
			return x;
		}

		double getY(){
			return y;
		}
};

class Node: public Point
{
	private:
		static   unsigned int num_node;
		unsigned int number;
		double       val;
		bool         is_on_boundary;
	
	public:
		Node():Point(){
			number = num_node++;
			val    = 0;
			is_on_boundary = false;
		}

		Node(double _x, double _y):Point(_x, _y){
			number = num_node++;
			val    = 0;
		}

        virtual ~Node() {}

		unsigned int getNum(){
			return number;
		}

		bool getIsOnBoundary(){
			return is_on_boundary;
		}

		void setNum( unsigned int _num){
            number = _num;
		}

		void setIsOnBoundary( bool _bool ){
			is_on_boundary = _bool;
		}

		bool isEqual( const Node& _node ){
			return (number == _node.number );
		}

        virtual bool isSuperNode(){
            return false;
        }
};

class SuperNode: public Node{
    public:
        SuperNode( double _x,double _y ):Node( _x,_y ){}
        virtual ~SuperNode () {}

        bool isSuperNode(){
            return true;
        }
};

#endif /* end of include guard */
