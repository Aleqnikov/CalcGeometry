#ifndef HULLCONTROLLER_H
#define HULLCONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QPolygonF>
#include <QPen>
#include <vector>

#include "views/NavigationView.h"

#include "../lib/api/GeometryApi.h"

class HullController : public QObject {
    Q_OBJECT
private:
    NavigationView *view_;
    QGraphicsScene *scene_;
    Hull hull_model_;
    bool useAndrew_;

public:
    HullController(NavigationView *view, QGraphicsScene *scene, Hull hull, bool useAndrew)
        : QObject(view), view_(view), scene_(scene),
        hull_model_(std::move(hull)), useAndrew_(useAndrew)
    {
        // Подключаем пробел для регенерации облака точек
        connect(view_, &NavigationView::spacePressed, this, &HullController::onSpacePressed);

        drawHull();
    }

private slots:
    void onSpacePressed() {
        hull_model_.Regenerate();
        drawHull();
    }

private:
    void drawHull() {
        scene_->clear();

        // 1. Рисуем все точки из облака (hull_.hull_)
        QPen pointPen(Qt::white);
        QBrush pointBrush(QColor("#00ff88"));
        double r = 0.1; // радиус точки

        for (const auto& p : hull_model_.hull_) {
            // Добавляем маленькие эллипсы для каждой точки
            scene_->addEllipse(p.x - r, p.y - r, r * 2, r * 2, Qt::NoPen, pointBrush);
        }

        // 2. Вычисляем оболочку в зависимости от выбранного метода
        std::vector<Point2D> convexPoints;
        if (useAndrew_) {
            convexPoints = hull_model_.ConvexAngrews();
        } else {
            convexPoints = hull_model_.ConvexJarvis();
        }

        // 3. Рисуем контур оболочки
        if (!convexPoints.empty()) {
            QPolygonF poly;
            for (const auto& p : convexPoints) {
                poly << QPointF(p.x, p.y);
            }

            QPen hullPen(QColor("#ff5555")); // Красная линия для оболочки
            hullPen.setWidth(2);
            hullPen.setCosmetic(true); // Чтобы линия не толстела при зуме

            // Рисуем замкнутую линию
            scene_->addPolygon(poly, hullPen, QBrush(QColor(255, 85, 85, 40)));
        }
    }
};

#endif // HULLCONTROLLER_H
