#ifndef RECTANGLECONTROLLER_H
#define RECTANGLECONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <vector>
#include "views/NavigationView.h"
#include "../lib/api/GeometryApi.h"

class RectangleController : public QObject {
    Q_OBJECT
private:
    NavigationView *view_;
    QGraphicsScene *scene_;
    int n_;

public:
    RectangleController(NavigationView *view, QGraphicsScene *scene, int n)
        : QObject(view), view_(view), scene_(scene), n_(n)
    {
        // Коннект на пробел для регенерации
        connect(view_, &NavigationView::spacePressed, this, &RectangleController::regenerateAndDraw);
        regenerateAndDraw();
    }

private slots:
    void regenerateAndDraw() {
        scene_->clear();

        // 1. Получаем данные через твой API
        Hull tempHull = CalcGeometryApi::CreateHull(n_);
        std::vector<Point2D> points = tempHull.hull_;
        BoundingRectangle::Rectangle rect = CalcGeometryApi::BaseMethod(points);

        // 2. Отрисовка точек (КОПИЯ СТАРОГО КОДА)
        QBrush brush(QColor("#ff5555"));
        for (const auto &p : points) {
            // Рисуем эллипс в локальных (-2, -2), чтобы центр был в 0
            auto *e = scene_->addEllipse(-2, -2, 4, 4, Qt::NoPen, brush);
            // Устанавливаем позицию (именно x_, y_ если они у тебя так называются)
            e->setPos(p.x, p.y);
            // КРИТИЧНО: чтобы точки не превращались в огромные круги при масштабе
            e->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
        }

        // 3. Отрисовка прямоугольника (КОПИЯ СТАРОГО КОДА)
        QPen pen(QColor("#00ff88"));
        pen.setWidth(2);
        pen.setCosmetic(true); // Чтобы линия не была толщиной в километр

        for (int i = 0; i < 4; ++i) {
            int j = (i + 1) % 4;

            scene_->addLine(rect.corners[i].x, rect.corners[i].y,
                            rect.corners[j].x, rect.corners[j].y, pen);
        }

        // Опционально: залей внутренность, если хочешь как на скрине
        QPolygonF poly;
        for(int i=0; i<4; ++i) poly << QPointF(rect.corners[i].x, rect.corners[i].y);
        scene_->addPolygon(poly, Qt::NoPen, QBrush(QColor(0, 255, 136, 40)));
    }
};

#endif
