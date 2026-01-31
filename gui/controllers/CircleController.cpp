#include "CircleController.h"

CircleController::CircleController(NavigationView *view, QGraphicsScene *scene,
                                   int n, Mode mode)
    : BaseController(view, scene), n_(n), mode_(mode)
{
    draw();
}

void CircleController::draw()
{
    clearScene();

    // Генерация точек
    Hull tempHull = CalcGeometryApi::CreateHull(n_);
    const auto& points = tempHull.hull_;

    // Отрисовка облака точек
    drawPoints(points, Colors::POINTS_ORANGE, 0.15);

    // Расчет и отрисовка окружностей
    if (mode_ == Naive || mode_ == CompareBoth) {
        auto circ = CalcGeometryApi::NaiveCircleAlg(points);
        drawCircle(circ.center, circ.radius, Colors::PRIMARY_GREEN, 2,
                   Qt::SolidLine, true, 20);
    }

    if (mode_ == Welzl || mode_ == CompareBoth) {
        auto circ = CalcGeometryApi::WelzAlg(points);
        Qt::PenStyle style = (mode_ == CompareBoth) ? Qt::DashLine : Qt::SolidLine;
        drawCircle(circ.center, circ.radius, Colors::SECONDARY_RED, 2,
                   style, true, 20);
    }

    fitViewToContent();
}
