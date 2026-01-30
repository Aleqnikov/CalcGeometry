#ifndef LINAL_H
#define LINAL_H


/**
* @breif В данном файле находится реализация основных операций из линейной алгебры
* которые необходимы для работы программы.
* А именно - пересечение ребра, дистанция, определение принадлежности треугольнику
* псевдоскаляное произведение, скалярное.
*/

#include <iostream>

#include "Point2D.h"


namespace LinAl {

    enum class Orient 
    {
        left,
        right,
        colin,
    };

    double pscalar( Point2D, Point2D);

    Orient orientation(Point2D, Point2D, Point2D);

    bool cross_rib(Point2D, Point2D, Point2D);

    double distance(const Point2D& a, const Point2D& b);

    bool isPointInTriangle(Point2D p, Point2D a, Point2D b, Point2D c);

    double dot(const Point2D& a, const Point2D& b);
}



#endif //LINAL_H
