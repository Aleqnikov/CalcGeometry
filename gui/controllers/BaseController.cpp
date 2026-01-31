#include "BaseController.h"

// Определение палитры
const QColor BaseController::Colors::POINTS_ORANGE = QColor("#FF8C00");
const QColor BaseController::Colors::PRIMARY_GREEN = QColor("#00ff88");
const QColor BaseController::Colors::SECONDARY_RED = QColor("#ff5555");
const QColor BaseController::Colors::ACCENT_BLUE = QColor("#3daee9");
const QColor BaseController::Colors::LABELS_YELLOW = QColor("#ffff00");

BaseController::BaseController(NavigationView *view, QGraphicsScene *scene, QObject *parent)
    : QObject(parent), view_(view), scene_(scene)
{
    // Подключаем виртуальный метод onSpacePressed
    connect(view_, &NavigationView::spacePressed, this, &BaseController::onSpacePressed);
}

void BaseController::onSpacePressed()
{
    // По умолчанию просто перерисовываем
    draw();
}

void BaseController::drawPoints(const std::vector<Point2D>& points, const QColor& color,
                                double radius, bool scaleIndependent)
{
    QBrush brush(color);
    for (const auto& p : points) {
        auto *ellipse = scene_->addEllipse(
            p.x - radius, p.y - radius,
            radius * 2, radius * 2,
            Qt::NoPen, brush
            );
        if (scaleIndependent) {
            ellipse->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
        }
    }
}

void BaseController::drawPolygon(const std::vector<Point2D>& points,
                                 const QPen& pen, const QBrush& brush)
{
    QPolygonF poly;
    for (const auto& p : points) {
        poly << QPointF(p.x, p.y);
    }
    scene_->addPolygon(poly, pen, brush);
}

void BaseController::drawLine(const Point2D& p1, const Point2D& p2,
                              const QColor& color, int lineWidth, Qt::PenStyle style)
{
    QPen pen = createPen(color, lineWidth, style);
    scene_->addLine(p1.x, p1.y, p2.x, p2.y, pen);

    // Точки на концах
    double r = 0.2;
    QBrush brush(color);
    scene_->addEllipse(p1.x - r, p1.y - r, r * 2, r * 2, Qt::NoPen, brush);
    scene_->addEllipse(p2.x - r, p2.y - r, r * 2, r * 2, Qt::NoPen, brush);
}

void BaseController::drawCircle(const Point2D& center, double radius,
                                const QColor& color, int lineWidth,
                                Qt::PenStyle style, bool withFill, int fillAlpha)
{
    QPen pen = createPen(color, lineWidth, style);
    QBrush brush = withFill ? createBrush(color, fillAlpha) : Qt::NoBrush;

    scene_->addEllipse(center.x - radius, center.y - radius,
                       radius * 2, radius * 2, pen, brush);

    // Точка центра
    double r = 0.2;
    scene_->addEllipse(center.x - r, center.y - r, r * 2, r * 2,
                       Qt::NoPen, QBrush(color));
}

void BaseController::drawRectangle(const Point2D corners[4], const QColor& color,
                                   int lineWidth, bool withFill, int fillAlpha)
{
    QPen pen = createPen(color, lineWidth);

    for (int i = 0; i < 4; ++i) {
        int j = (i + 1) % 4;
        scene_->addLine(corners[i].x, corners[i].y,
                        corners[j].x, corners[j].y, pen);
    }

    if (withFill) {
        QPolygonF poly;
        for (int i = 0; i < 4; ++i) {
            poly << QPointF(corners[i].x, corners[i].y);
        }
        scene_->addPolygon(poly, Qt::NoPen, createBrush(color, fillAlpha));
    }
}

QPen BaseController::createPen(const QColor& color, int width,
                               Qt::PenStyle style, bool cosmetic)
{
    QPen pen(color);
    pen.setWidth(width);
    pen.setStyle(style);
    pen.setCosmetic(cosmetic);
    return pen;
}

QBrush BaseController::createBrush(const QColor& color, int alpha)
{
    QColor fillColor = color;
    fillColor.setAlpha(alpha);
    return QBrush(fillColor);
}

void BaseController::clearScene()
{
    scene_->clear();
}

void BaseController::fitViewToContent()
{
    QRectF rect;
    for (auto item : scene_->items()) {
        if (item->zValue() < 100) { // или item != indicator_
            rect |= item->sceneBoundingRect();
        }
    }
    if (!rect.isNull()) {
        view_->fitInView(rect, Qt::KeepAspectRatio);
    }
}
