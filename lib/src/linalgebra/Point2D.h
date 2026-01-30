#ifndef POINT2D_H
#define POINT2D_H


/**
* @breif Структура, которая определеят двумерную точку, для нее определены все
* необходимые опреаци.
*/


struct Point2D
{
	double x, y;

	Point2D(double x = 0, double y = 0) : x(x), y(y) {};

	Point2D operator+(const Point2D& point) const
	{
		return Point2D (x + point.x, y + point.y);
	}

	Point2D operator-(const Point2D& point) const 
	{
		return Point2D(x - point.x, y - point.y);
	}

	bool operator!=(const Point2D& point) const 
	{
		return !(*this == point);
	}


	bool operator==(const Point2D& point) const 
	{
		return x == point.x && y == point.y;
	}

	bool operator<(const Point2D& point) const 
	{
		return (x < point.x) || (x == point.x && y < point.y);
	}

	Point2D operator*(double k) const 
	{
		return { x * k, y * k };
	}
};


#endif //POINT2D_H
