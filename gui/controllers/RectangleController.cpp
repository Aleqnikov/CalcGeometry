#include "RectangleController.h"

RectangleController::RectangleController(NavigationView *view, QGraphicsScene *scene, int n)
    : BaseController(view, scene), n_(n)
{
    draw();
}

void RectangleController::draw()
{
    clearScene();

    // Генерация данных
    Hull tempHull = CalcGeometryApi::CreateHull(n_);
    const auto& points = tempHull.hull_;
    BoundingRectangle::Rectangle rect = CalcGeometryApi::BaseMethod(points);

    // Отрисовка точек с флагом scaleIndependent = true
    drawPoints(points, Colors::SECONDARY_RED, 1.0, false);  // true = не масштабируются!

    // Отрисовка прямоугольника
    drawRectangle(rect.corners, Colors::PRIMARY_GREEN, 2, true, 40);

    fitViewToContent();
}
