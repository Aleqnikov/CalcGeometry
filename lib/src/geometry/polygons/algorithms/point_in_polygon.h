#ifndef POINTINPOLYGONS_H
#define POINTINPOLYGONS_H

/**
 * @brief В данном файле находятся все алгоритмы для определения принадлежности точки
 * многоугольнику.
 */

#include "../../../linalgebra/LinAl.h"


namespace PointInPn 
{

    /**
     * @brief Проверяет что точка лежит внутри звезного многоугольника при помощи бинарного поиска 
     * сектора.
     */
    bool InStarPn (
                    Point2D,
                    Point2D,
                    std::vector<Point2D>::const_iterator,
                    std::vector<Point2D>::const_iterator
                    );

    /**
     * @brief Проверяет, что точка лежит внутри многоугольника при помощи трассируюшей прямой.
     */
    bool InAllPn (
                    Point2D,
                    std::vector<Point2D>::const_iterator,
                    std::vector<Point2D>::const_iterator
                    );

}



#endif //POINTINPOLYGONS_H
