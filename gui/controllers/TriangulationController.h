#ifndef TRIANGULATIONCONTROLLER_H
#define TRIANGULATIONCONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include <QPen>
#include <QPolygonF>
#include <vector>
#include "views/NavigationView.h"

#include "../lib/api/GeometryApi.h"

class TriangulationController : public QObject {
    Q_OBJECT
public:
    TriangulationController(NavigationView *view, QGraphicsScene *scene,
                            const std::vector<Point2D>& vertices,
                            const std::vector<Triangle>& triangles)
        : QObject(view), scene_(scene)
    {
        draw(vertices, triangles);
    }

private:
    QGraphicsScene *scene_;

    void draw(const std::vector<Point2D>& vertices, const std::vector<Triangle>& triangles) {
        scene_->clear();

        // 1. Рисуем треугольники
        QPen triPen(QColor("#3daee9")); // Голубые границы треугольников
        triPen.setWidth(1);
        triPen.setCosmetic(true);
        QBrush triBrush(QColor(61, 174, 233, 40)); // Легкая заливка

        for (const auto& t : triangles) {
            QPolygonF triPoly;
            // Используем твои p1, p2, p3
            triPoly << QPointF(t.p1.x, t.p1.y)
                    << QPointF(t.p2.x, t.p2.y)
                    << QPointF(t.p3.x, t.p3.y);
            scene_->addPolygon(triPoly, triPen, triBrush);
        }

        // 2. Рисуем внешний контур полигона
        QPen contourPen(QColor("#00ff88")); // Ярко-зеленый контур
        contourPen.setWidth(2);
        contourPen.setCosmetic(true);

        QPolygonF contour;
        for (const auto& v : vertices) {
            contour << QPointF(v.x, v.y);
        }
        scene_->addPolygon(contour, contourPen, Qt::NoBrush);
    }
};

#endif
