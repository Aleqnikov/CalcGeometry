#ifndef BOUNDING_CIRCLE_H
#define BOUNDING_CIRCLE_H

/**
 * @brief В данном файле находятся функции для пострения ограничиваюшей окружности.
 */

#include "../../../linalgebra/LinAl.h"

#include <cmath>
#include <algorithm>
#include <random>

namespace BoundingCircle {

    struct Circle {
        Point2D center;
        double radius;

        Circle() : center(0, 0), radius(0) {}
        Circle(Point2D c, double r) : center(c), radius(r) {}

        bool contains(const Point2D& p) const {
            double dx = p.x - center.x;
            double dy = p.y - center.y;
            return (dx * dx + dy * dy) <= radius * radius + 1e-9;
        }
    };

    Circle NaiveAlgorithm(
        std::vector<Point2D>::const_iterator begin,
        std::vector<Point2D>::const_iterator end);

    Circle WelzlAlgorithm(
        std::vector<Point2D>::const_iterator begin,
        std::vector<Point2D>::const_iterator end);


    Circle enclosingCircle(const std::vector<Point2D>& boundary);
    Circle circleFrom2Points(const Point2D& a, const Point2D& b);
    Circle circleFrom3Points(const Point2D& a, const Point2D& b, const Point2D& c);


    double circlePredicate(const Point2D& a, const Point2D& b, 
                          const Point2D& c, const Point2D& p);

}

#endif // BOUNDING_CIRCLE_H