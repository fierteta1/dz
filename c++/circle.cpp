#include <iostream>
#include <cmath>

using namespace std;

class Point {
	double x,y;
public:
	Point(double _x = 0, double _y = 0) {x = _x; y = _y;}
	static double Line(Point t1, Point t2) {retutrn sqrt(pow((t1.x - t2.x),2) + pow((t1.y - t2.y),2)); }
};

class Circle {
	Point centre;
	double rad;
public:
	Circle(Point _centre, double _rad) {centre = _centre; rad = _rad; }
	bool Point_Circl(Point t) {return Point::Line(centre, t) <= rad; }
};

int main()
{
	Point t1(5, 1);
	Point centre(0, 0);
	double r = 5;
	Circle cl(centre, r);
	
	cout << cl.Point_Circl(t1) << endl;

	return 0;
}
