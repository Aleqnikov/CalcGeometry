#include "TriangulationController.h"

TriangulationController::TriangulationController(NavigationView *view,
                                                 QGraphicsScene *scene,
                                                 const std::vector<Point2D>& vertices,
                                                 const std::vector<Triangle>& triangles)
    : BaseController(view, scene), vertices_(vertices), triangles_(triangles)
{
    draw();
}

void TriangulationController::draw()
{
    clearScene();

    // Отрисовка треугольников
    QPen triPen = createPen(Colors::ACCENT_BLUE, 1);
    QBrush triBrush = createBrush(Colors::ACCENT_BLUE, 40);

    for (const auto& t : triangles_) {
        std::vector<Point2D> triPoints = {t.p1, t.p2, t.p3};
        drawPolygon(triPoints, triPen, triBrush);
    }

    // Отрисовка внешнего контура
    QPen contourPen = createPen(Colors::PRIMARY_GREEN, 2);
    drawPolygon(vertices_, contourPen, Qt::NoBrush);

    fitViewToContent();
}
