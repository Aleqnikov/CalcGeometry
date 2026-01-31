#ifndef CIRCLECONTROLLER_H
#define CIRCLECONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include <QPen>
#include <vector>
#include "views/NavigationView.h"
#include "../lib/api/GeometryApi.h"

class CircleController : public QObject {
    Q_OBJECT
public:
    enum Mode { Naive, Welzl, CompareBoth };

private:
    NavigationView *view_;
    QGraphicsScene *scene_;
    int n_;
    Mode mode_;

public:
    CircleController(NavigationView *view, QGraphicsScene *scene, int n, Mode mode)
        : QObject(view), view_(view), scene_(scene), n_(n), mode_(mode)
    {
        connect(view_, &NavigationView::spacePressed, this, &CircleController::onSpacePressed);
        regenerateAndDraw();
    }

private slots:
    void onSpacePressed() {
        regenerateAndDraw();
    }

private:
    void drawCircle(const BoundingCircle::Circle& c, QColor color, Qt::PenStyle style = Qt::SolidLine) {
        QPen pen(color);
        pen.setWidth(2);
        pen.setCosmetic(true);
        pen.setStyle(style);

        // В Qt отрисовка идет от верхнего левого угла, а не от центра
        double r = c.radius;
        scene_->addEllipse(c.center.x - r, c.center.y - r, r * 2, r * 2, pen, QBrush(QColor(color.red(), color.green(), color.blue(), 20)));

        // Рисуем точку центра
        scene_->addEllipse(c.center.x - 0.2, c.center.y - 0.2, 0.4, 0.4, Qt::NoPen, QBrush(color));
    }

    void regenerateAndDraw() {
        scene_->clear();

        // 1. Генерация точек через API
        Hull tempHull = CalcGeometryApi::CreateHull(n_);
        std::vector<Point2D> points = tempHull.hull_;

        // 2. Отрисовка облака
        QBrush pointBrush(QColor("#FF8C00"));
        for (const auto& p : points) {
            scene_->addEllipse(p.x - 0.15, p.y - 0.15, 0.3, 0.3, Qt::NoPen, pointBrush);
        }

        // 3. Расчет и отрисовка окружностей через API
        if (mode_ == Naive || mode_ == CompareBoth) {
            auto circ = CalcGeometryApi::NaiveCircleAlg(points);
            drawCircle(circ, QColor("#00ff88")); // Зеленая
        }

        if (mode_ == Welzl || mode_ == CompareBoth) {
            auto circ = CalcGeometryApi::WelzAlg(points);
            drawCircle(circ, QColor("#ff5555"), (mode_ == CompareBoth ? Qt::DashLine : Qt::SolidLine)); // Красная (пунктир в сравнении)
        }
    }
};

#endif
