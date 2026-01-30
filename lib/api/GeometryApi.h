#ifndef GEOMETRYAPI_H
#define GEOMETRYAPI_H

/**
 * @brief Api для взаимодействия c бибилиотекой.
 */


#include "../src/linalgebra/LinAl.h"

#include "../src/geometry/polygons/core/ConvexPn.h"
#include "../src/geometry/polygons/core/StarPn.h"
#include "../src/geometry/polygons/core/Polygon.h"

#include "../src/geometry/boundingsets/algorithms/BoundingCircle.h"
#include "../src/geometry/boundingsets/algorithms/BoundingRectangle.h"
#include "../src/geometry/boundingsets/algorithms/DiameterAlgorithms.h"

#include "../src/geometry/convexhull/core/Hull.h"

#include "../src/geometry/triangulation/algorithms/TriangulationAlgorithms.h"

namespace CalcGeometryApi {
    

    /**
     * Функции для определения принадлежности точки многоугольнику.
     * 
     * Предполагается что пользователь должен получить на выходе многоугольник, и затем уже использовать его методы для взаимодействия.
     */

    ConvexPolygon CreatreConvexPolygon(int);
    Polygon CreatrePolygon(int);
    StarPolygon CreatreStarPolygon(int);


    /**
     * Функции для построения ограничиваюшей оболочки.
     * 
     * @brief Предполаается, что человек на вход подает само ограничиваюшее множество, и на выход получат фигуру, того типа что названа функция.
     */

    BoundingCircle::Circle NaiveCircleAlg(std::vector<Point2D>);
    BoundingCircle::Circle WelzAlg(std::vector<Point2D>);

    BoundingRectangle::Rectangle BaseMethod(std::vector<Point2D>);

    std::pair<Point2D, Point2D> RotatingCallipers(std::vector<Point2D>);
    std::pair<Point2D, Point2D> HullAlg(std::vector<Point2D>);
    std::pair<Point2D, Point2D> NaiveDiam(std::vector<Point2D>);


    /**
     * Построение выпуклой оболочки.
     * 
     * @brief Предполагается, что пользователь на выходе получает экземпляр класса оболочки, и уже из нее строит полученную оболочку.
     */

    Hull CreateHull(int);


    /**
     * Триангуляция прямоегольника.
     * 
     * @brief Предполагается, что пользователь передает вектор точек, и затем он получает для него список треугольников.
     */

    std::vector<Triangle> EarClipping(std::vector<Point2D>);
    
}



#endif //GEOMETRYAPI_H
