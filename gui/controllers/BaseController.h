#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QPolygonF>
#include "views/NavigationView.h"
#include "../lib/api/GeometryApi.h"

class BaseController : public QObject {
    Q_OBJECT

public:
    explicit BaseController(NavigationView *view, QGraphicsScene *scene, QObject *parent = nullptr);
    virtual ~BaseController() = default;

protected:
    // Общие утилиты отрисовки
    void drawPoints(const std::vector<Point2D>& points, const QColor& color,
                    double radius = 0.15, bool scaleIndependent = false);

    void drawPolygon(const std::vector<Point2D>& points, const QPen& pen,
                     const QBrush& brush = Qt::NoBrush);

    void drawLine(const Point2D& p1, const Point2D& p2, const QColor& color,
                  int lineWidth = 2, Qt::PenStyle style = Qt::SolidLine);

    void drawCircle(const Point2D& center, double radius, const QColor& color,
                    int lineWidth = 2, Qt::PenStyle style = Qt::SolidLine,
                    bool withFill = false, int fillAlpha = 20);

    void drawRectangle(const Point2D corners[4], const QColor& color,
                       int lineWidth = 2, bool withFill = false, int fillAlpha = 40);

    QPen createPen(const QColor& color, int width = 2,
                   Qt::PenStyle style = Qt::SolidLine, bool cosmetic = true);

    QBrush createBrush(const QColor& color, int alpha = 255);

    void clearScene();
    void fitViewToContent();

    // Виртуальные методы для переопределения
    virtual void draw() = 0;
    virtual void onSpacePressed();  // НОВОЕ: виртуальный метод для пробела

    NavigationView *view_;
    QGraphicsScene *scene_;

    // Палитра цветов
    struct Colors {
        static const QColor POINTS_ORANGE;
        static const QColor PRIMARY_GREEN;
        static const QColor SECONDARY_RED;
        static const QColor ACCENT_BLUE;
        static const QColor LABELS_YELLOW;
    };
};

#endif
