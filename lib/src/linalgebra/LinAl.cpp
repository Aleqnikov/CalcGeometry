#include "LinAl.h"


double LinAl::pscalar(Point2D a, Point2D b) 
{
	return a.x * b.y - a.y * b.x;
}

LinAl::Orient LinAl::orientation(Point2D a, Point2D b, Point2D c) 
{
	double ps = pscalar(b - a, c - a);

	if (ps > 0) return Orient::left;

	if (ps < 0) return Orient::right;

	return Orient::colin;
}


bool LinAl::cross_rib(Point2D a, Point2D b, Point2D p) 
{
    if(a.y > b.y) std::swap(a, b);

    if(p.y <= a.y || p.y > b.y) return false;

    Orient o1 = orientation(a, b, p);

    return o1 == Orient::right;
}

double LinAl::distance(const Point2D& a, const Point2D& b) 
{
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return dx * dx + dy * dy;
}

bool LinAl::isPointInTriangle(Point2D p, Point2D a, Point2D b, Point2D c) 
{
    Orient o1 = orientation(a, b, p);
    Orient o2 = orientation(b, c, p);
    Orient o3 = orientation(c, a, p);
    
    return (o1 == o2 && o2 == o3);
}

double LinAl::dot(const Point2D& a, const Point2D& b)
{
    return a.x * b.x + a.y * b.y;
}