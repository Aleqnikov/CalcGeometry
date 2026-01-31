#include "DiameterController.h"

DiameterController::DiameterController(NavigationView *view, QGraphicsScene *scene,
                                       int n, Mode mode)
    : BaseController(view, scene), n_(n), mode_(mode)
{
    draw();
}

void DiameterController::draw()
{
    clearScene();

    // Генерация точек
    Hull tempHull(n_);
    const auto& points = tempHull.hull_;

    // Отрисовка облака
    drawPoints(points, Colors::POINTS_ORANGE, 0.15);

    // Расчет и отрисовка диаметров
    if (mode_ == Naive) {
        auto d = CalcGeometryApi::NaiveDiam(points);
        drawLine(d.first, d.second, Colors::PRIMARY_GREEN);
    }
    else if (mode_ == Calipers) {
        auto d = CalcGeometryApi::RotatingCallipers(points);
        drawLine(d.first, d.second, Colors::SECONDARY_RED);
    }
    else if (mode_ == CompareBoth) {
        auto d1 = CalcGeometryApi::NaiveDiam(points);
        auto d2 = CalcGeometryApi::HullAlg(points);
        drawLine(d1.first, d1.second, Colors::PRIMARY_GREEN, 2, Qt::SolidLine);
        drawLine(d2.first, d2.second, Colors::SECONDARY_RED, 2, Qt::DashLine);
    }

    fitViewToContent();
}
