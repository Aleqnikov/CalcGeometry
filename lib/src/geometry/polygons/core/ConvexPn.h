#ifndef CONVEx_POLYGON_H
#define CONVEx_POLYGON_H

#include <set>
#include <random>
#include <cmath>

#include "../algorithms/point_in_polygon.h"

class ConvexPolygon{
public:
	Point2D centroid;
    std::vector<Point2D> vertices;

    ConvexPolygon(int);
    void Regenerate();
    bool inPolygon(Point2D);
};


#endif //CONVEx_POLYGON_H
