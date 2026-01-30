#ifndef BOUNDING_RECTANGLE_H
#define BOUNDING_RECTANGLE_H

/**
 * @brief В данном файле находятся функции для пострения ограничиваюшего четырёхугольника.
 */



#include "../../../linalgebra/LinAl.h"
#include "../../convexhull/algorithms/HullAlgorithms.h"


#include <vector>
#include <cmath>
#include <limits>

namespace BoundingRectangle {

    struct Rectangle {
        Point2D corners[4];  
        double area;
        double perimeter;

        Rectangle() : area(0), perimeter(0) {
            for (int i = 0; i < 4; i++)
                corners[i] = Point2D(0, 0);
        }
    };


    Rectangle RotatingCalipers(
        std::vector<Point2D>::const_iterator begin,
        std::vector<Point2D>::const_iterator end);

}

#endif // BOUNDING_RECTANGLE_H