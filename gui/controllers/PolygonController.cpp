#include "PolygonController.h"

PolygonController::PolygonController(NavigationView *view, QGraphicsScene *scene,
                                     PolygonVariant polygon)
    : BaseController(view, scene), polygon_(std::move(polygon))
{
    indicator_ = new QGraphicsTextItem();
    indicator_->setZValue(100);
    indicator_->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
    scene_->addItem(indicator_);

    connect(view_, &NavigationView::mouseMovedTo, this, &PolygonController::onMouseMoved);

    draw();
}

void PolygonController::onSpacePressed()
{
    std::visit([](auto& poly) { poly.Regenerate(); }, polygon_);
    draw();
}

void PolygonController::draw()
{
    // Очистка всего кроме индикатора
    for (auto item : scene_->items()) {
        if (item != indicator_) {
            scene_->removeItem(item);
            delete item;
        }
    }

    // Отрисовка полигона
    std::vector<Point2D> vertices;
    std::visit([&](const auto& poly) {
        vertices = poly.vertices;
    }, polygon_);

    QPen pen = createPen(Colors::PRIMARY_GREEN, 0);
    drawPolygon(vertices, pen, Qt::NoBrush);


    // Метки вершин
    drawVertexLabels();

    fitViewToContent();
}

void PolygonController::onMouseMoved(QPointF scenePos)
{
    bool isInside = std::visit([&](auto& poly) -> bool {
        return poly.inPolygon(Point2D(scenePos.x(), scenePos.y()));
    }, polygon_);

    updateIndicator(scenePos, isInside);
}

void PolygonController::updateIndicator(QPointF pos, bool isInside)
{
    indicator_->setPlainText(isInside ? "TRUE" : "FALSE");
    indicator_->setDefaultTextColor(
        isInside ? Colors::PRIMARY_GREEN : Colors::SECONDARY_RED
        );
    indicator_->setPos(pos.x(), pos.y());

    QFont font = indicator_->font();
    font.setPointSize(14);
    font.setBold(true);
    indicator_->setFont(font);
}

void PolygonController::drawVertexLabels()
{
    QFont font;
    font.setPointSize(9);
    font.setBold(true);

    std::visit([&](const auto& poly) {
        for (int i = 0; i < static_cast<int>(poly.vertices.size()); ++i) {
            const auto& v = poly.vertices[i];
            QGraphicsTextItem *text = scene_->addText(QString::number(i), font);
            text->setDefaultTextColor(Colors::LABELS_YELLOW);
            text->setPos(v.x, v.y);
            text->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
        }
    }, polygon_);
}
