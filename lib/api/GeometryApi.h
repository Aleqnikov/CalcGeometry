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

#include "../src/geometry/delone/algorithm/DelaunayTriangulation.h"

namespace CalcGeometryApi {

    ConvexPolygon CreatreConvexPolygon(int);
    Polygon CreatrePolygon(int);
    StarPolygon CreatreStarPolygon(int);

    BoundingCircle::Circle NaiveCircleAlg(std::vector<Point2D>);
    BoundingCircle::Circle WelzAlg(std::vector<Point2D>);

    BoundingRectangle::Rectangle BaseMethod(std::vector<Point2D>);

    std::pair<Point2D, Point2D> RotatingCallipers(std::vector<Point2D>);
    std::pair<Point2D, Point2D> HullAlg(std::vector<Point2D>);
    std::pair<Point2D, Point2D> NaiveDiam(std::vector<Point2D>);

    Hull CreateHull(int);

    std::vector<Triangle> EarClipping(std::vector<Point2D>);
    std::vector<Triangle> Delane(std::vector<Point2D>);
}

#endif // GEOMETRYAPI_H