#ifndef POINT_H
#define POINT_H

class Point
{
	private:
		double x,y;

	public:
		Point():x(0),y(0){}
		Point( double _x, double _y ):x(_x),y(_y){}

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
	
	public:
		Node():Point(){
			number = num_node++;
			val    = 0;
		}

		Node(double _x, double _y):Point(_x, _y){
			number = num_node++;
			val    = 0;
		}

		unsigned int getNum(){
			return number;
		}
};

#endif /* end of include guard */
