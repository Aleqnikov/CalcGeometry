#ifndef DIAMETERCONTROLLER_H
#define DIAMETERCONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include <QPen>
#include <vector>
#include "views/NavigationView.h"

#include "../lib/api/GeometryApi.h"


class DiameterController : public QObject {
    Q_OBJECT
public:
    enum Mode { Naive, Calipers, CompareBoth };

private:
    NavigationView *view_;
    QGraphicsScene *scene_;
    int n_;
    Mode mode_;

public:
    DiameterController(NavigationView *view, QGraphicsScene *scene, int n, Mode mode)
        : QObject(view), view_(view), scene_(scene), n_(n), mode_(mode)
    {
        connect(view_, &NavigationView::spacePressed, this, &DiameterController::onSpacePressed);
        regenerateAndDraw();
    }

private slots:
    void onSpacePressed() {
        regenerateAndDraw();
    }

private:
    void drawLine(const std::pair<Point2D, Point2D>& d, QColor color, Qt::PenStyle style = Qt::SolidLine) {
        QPen pen(color);
        pen.setWidth(2);
        pen.setCosmetic(true);
        pen.setStyle(style);
        scene_->addLine(d.first.x, d.first.y, d.second.x, d.second.y, pen);

        // Точки на концах
        scene_->addEllipse(d.first.x - 0.2, d.first.y - 0.2, 0.4, 0.4, Qt::NoPen, QBrush(color));
        scene_->addEllipse(d.second.x - 0.2, d.second.y - 0.2, 0.4, 0.4, Qt::NoPen, QBrush(color));
    }

    void regenerateAndDraw() {
        scene_->clear();

        // 1. Генерация точек
        Hull tempHull(n_);
        std::vector<Point2D> points = tempHull.hull_;

        QBrush pointBrush(QColor("#FF8C00"));
        double r = 0.15;
        for (const auto& p : points) {
            scene_->addEllipse(p.x - r, p.y - r, r * 2, r * 2, Qt::NoPen, pointBrush);
        }

        // 3. Логика по режимам
        if (mode_ == Naive) {
            auto d = CalcGeometryApi::NaiveDiam(points);
            drawLine(d, QColor("#00ff88")); // Зеленый
        }
        else if (mode_ == Calipers) {
            auto d = CalcGeometryApi::RotatingCallipers(points);
            drawLine(d, QColor("#ff5555")); // Красный
        }
        else if (mode_ == CompareBoth) {
            auto d1 = CalcGeometryApi::NaiveDiam(points);
            auto d2 = CalcGeometryApi::HullAlg(points);

            drawLine(d1, QColor("#00ff88")); // Зеленый (сплошной)
            drawLine(d2, QColor("#ff5555"), Qt::DashLine); // Красный (пунктир)
        }
    }
};

#endif
