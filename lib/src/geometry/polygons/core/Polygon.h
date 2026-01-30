#ifndef POLYGON_H
#define POLYGON_H

/**
 * @brief Класс произвольного многоугольника, здесь уже так называемый интерфейс
 * взаимодействия алгоритмов и самой программы.
 */

#include <random>
#include <cmath>

#include "../algorithms/point_in_polygon.h"

class Polygon {
public:
    std::vector<Point2D> vertices;

    Polygon(int);
    void Regenerate();

    bool inPolygon(Point2D);

};

#endif //POLYGON_H
