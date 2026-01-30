#ifndef POLYGONCONTROLLER_H
#define POLYGONCONTROLLER_H

#include <QObject>
#include <QPainter>
#include <QGraphicsScene>
#include <variant>
#include <QGraphicsTextItem>
#include <QGraphicsItem>
#include <QApplication>

#include "NavigationView.h"
#include "../lib/api/GeometryApi.h"

using PolygonVariant = std::variant<Polygon, ConvexPolygon, StarPolygon>;

class PolygonController : public QObject {
    Q_OBJECT
private:
    NavigationView *view_;
    QGraphicsScene *scene_;
    PolygonVariant polygon_;

    QGraphicsTextItem *indicator_;

public:
    PolygonController(NavigationView *view, QGraphicsScene *scene, PolygonVariant polygon)
        : QObject(view), view_(view), scene_(scene), polygon_(std::move(polygon))
    {
        indicator_ = new QGraphicsTextItem();
        indicator_->setZValue(100); 
        indicator_->setFlag(QGraphicsItem::ItemIgnoresTransformations, true); 
        scene_->addItem(indicator_);

        connect(view_, &NavigationView::mouseMovedTo, this, &PolygonController::onMouseMoved);
        connect(view_, &NavigationView::spacePressed, this, &PolygonController::onSpacePressed);

        drawPolygon();
    }

private slots:
    void onMouseMoved(QPointF scenePos) {
        bool isInside = std::visit([&](auto& poly) -> bool { return poly.inPolygon(Point2D(scenePos.x(), scenePos.y())); }, polygon_);

        updateIndicator(scenePos, isInside);
    }

    void onSpacePressed() {
        std::visit([](auto& poly) { poly.Regenerate(); }, polygon_);

        drawPolygon();
    }

private:
    void updateIndicator(QPointF pos, bool isInside) {
        if (!indicator_) return;

        indicator_->setPlainText(isInside ? "TRUE" : "FALSE");
        indicator_->setDefaultTextColor(isInside ? QColor("#00ff88") : QColor("#ff5555"));

        indicator_->setPos(pos.x(), pos.y());

        QFont font = indicator_->font();
        font.setPointSize(14);
        font.setBold(true);
        indicator_->setFont(font);
    }

    void drawPolygon() {
        for (auto item : scene_->items()) {
            if (item != indicator_) scene_->removeItem(item);
        }

        if (!scene_->items().contains(indicator_)) scene_->addItem(indicator_);

        QPolygonF qpoly;
        std::visit([&](auto& poly) {
            for (const auto &v : poly.vertices)
                qpoly << QPointF(v.x, v.y); 
        }, polygon_);

        if (!qpoly.isEmpty()) {
            QPen pen(QColor("#00ff88"));
            pen.setWidth(0);
            pen.setCosmetic(true);
            scene_->addPolygon(qpoly, pen);
        }


        QFont font;
        font.setPointSize(9);
        font.setBold(true);

        std::visit([&](auto& poly) {
            for (int i = 0; i < (int)poly.vertices.size(); ++i) {
                const auto &v = poly.vertices[i];
                QGraphicsTextItem *text = scene_->addText(QString::number(i), font);
                text->setDefaultTextColor(QColor("#ffff00"));
                text->setPos(v.x, v.y);
                text->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
            }
        }, polygon_);
    }
};

#endif
