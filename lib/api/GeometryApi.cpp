#include "GeometryApi.h"


// Функции для построения многоугольника.

ConvexPolygon CalcGeometryApi::CreatreConvexPolygon(int cnt_pts)
{
    return ConvexPolygon(cnt_pts);
}


Polygon CalcGeometryApi::CreatrePolygon(int cnt_pts)
{
    return Polygon(cnt_pts);
}

StarPolygon CalcGeometryApi::CreatreStarPolygon(int cnt_pts)
{
    return StarPolygon(cnt_pts);
}


// Функции для построения ограничиваюшей оболочки.

BoundingCircle::Circle CalcGeometryApi::NaiveCircleAlg(std::vector<Point2D> mesh)
{
    return BoundingCircle::NaiveAlgorithm(mesh.begin(), mesh.end());
}

BoundingCircle::Circle CalcGeometryApi::WelzAlg(std::vector<Point2D> mesh)
{
    return BoundingCircle::WelzlAlgorithm(mesh.begin(), mesh.end());
}


BoundingRectangle::Rectangle CalcGeometryApi::BaseMethod(std::vector<Point2D> mesh) 
{
    return BoundingRectangle::RotatingCalipers(mesh.begin(), mesh.end());
}



std::pair<Point2D, Point2D> CalcGeometryApi::RotatingCallipers(std::vector<Point2D> mesh)
{
    return DiameterAlgorithms::RotatingCalipers(mesh.begin(), mesh.end());
}

std::pair<Point2D, Point2D> CalcGeometryApi::HullAlg(std::vector<Point2D> mesh)
{
    return DiameterAlgorithms::DiameterWithHull(mesh.begin(), mesh.end());
}

std::pair<Point2D, Point2D> CalcGeometryApi::NaiveDiam(std::vector<Point2D> mesh)
{
    return DiameterAlgorithms::NaiveAlgorithm(mesh.begin(), mesh.end());
}


// Построение выпуклой оболочки.
Hull CalcGeometryApi::CreateHull(int cnt_pts)
{
    return Hull(cnt_pts);
}

// Триангуляция многоугольника.
std::vector<Triangle> CalcGeometryApi::EarClipping(std::vector<Point2D>  vercites)
{
    return Triangulation::EarClipping(vercites);
}