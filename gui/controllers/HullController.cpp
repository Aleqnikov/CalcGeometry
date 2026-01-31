#include "HullController.h"

HullController::HullController(NavigationView *view, QGraphicsScene *scene,
                               Hull hull, bool useAndrew)
    : BaseController(view, scene), hullModel_(std::move(hull)), useAndrew_(useAndrew)
{
    draw();
}

void HullController::onSpacePressed()
{
    hullModel_.Regenerate();  // Специфичная логика
    draw();
}

void HullController::draw()
{
    clearScene();

    // Отрисовка облака точек
    drawPoints(hullModel_.hull_, Colors::PRIMARY_GREEN, 0.1);

    // Вычисление оболочки
    std::vector<Point2D> convexPoints = useAndrew_
                                            ? hullModel_.ConvexAngrews()
                                            : hullModel_.ConvexJarvis();

    // Отрисовка оболочки
    if (!convexPoints.empty()) {
        QPen pen = createPen(Colors::SECONDARY_RED, 2);
        QBrush brush = createBrush(Colors::SECONDARY_RED, 40);
        drawPolygon(convexPoints, pen, brush);
    }

    fitViewToContent();
}
