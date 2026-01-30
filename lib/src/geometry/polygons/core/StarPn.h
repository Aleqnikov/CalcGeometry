#ifndef STAR_POLYGON_H
#define STAR_POLYGON_H

#include <set>
#include <random>
#include <cmath>

#include "../algorithms/point_in_polygon.h"

class StarPolygon{
public:
    std::vector<Point2D> vertices;

    StarPolygon(int);
    void Regenerate();
    bool inPolygon(Point2D);
};


#endif //STAR_POLYGON_H
