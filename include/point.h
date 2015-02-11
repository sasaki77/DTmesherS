#ifndef POINT_H
#define POINT_H

class Point{
	private:
		double x,y;

	public:
		Point():x(0),y(0){}
		Point( double _x, double _y ):x(_x),y(_y){}

		void set(double _x, double _y){
			x = _x;
			y = _y;
		}

		double getX(){
			return x;
		}

		double getY(){
			return y;
		}
};


#endif /* end of include guard */
