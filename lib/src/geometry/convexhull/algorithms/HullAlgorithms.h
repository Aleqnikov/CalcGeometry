#ifndef HULLALGORITHMS_H
#define HULLALGORITHMS_H

/**
 * @brief В данном файле находятся алгоритмы по пострению выпуклой оболочки.
 * Такие как Jasvis March, Graham.
 */

#include "../../../linalgebra/LinAl.h"

namespace HullAlgs
{
    std::vector<Point2D> JarvisMarch (  std::vector<Point2D>::const_iterator,
                                        std::vector<Point2D>::const_iterator);

    std::vector<Point2D> Andrews (  std::vector<Point2D>::const_iterator,
                                    std::vector<Point2D>::const_iterator);
}



#endif //HULLALGORITHMS_H
