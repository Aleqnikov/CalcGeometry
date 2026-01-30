#ifndef DIAMETER_ALGORITHMS_H
#define DIAMETER_ALGORITHMS_H

/**
 * @brief В данном файле находятся функции для поиска диаметра множества.
 */

#include "../../../linalgebra/LinAl.h"
#include "../../convexhull/algorithms/HullAlgorithms.h"

#include <vector>
#include <utility>
#include <cmath>
#include <limits>

namespace DiameterAlgorithms {

    std::pair<Point2D, Point2D> NaiveAlgorithm(
        std::vector<Point2D>::const_iterator begin,
        std::vector<Point2D>::const_iterator end);

    std::pair<Point2D, Point2D> RotatingCalipers(
        std::vector<Point2D>::const_iterator begin,
        std::vector<Point2D>::const_iterator end);

    std::pair<Point2D, Point2D> DiameterWithHull(
        std::vector<Point2D>::const_iterator begin,
        std::vector<Point2D>::const_iterator end);

}

#endif // DIAMETER_ALGORITHMS_H