#ifndef POLYGONCONTROLLER_H
#define POLYGONCONTROLLER_H

#include "BaseController.h"
#include <variant>
#include <QGraphicsTextItem>

using PolygonVariant = std::variant<Polygon, ConvexPolygon, StarPolygon>;

class PolygonController : public BaseController {
    Q_OBJECT

public:
    PolygonController(NavigationView *view, QGraphicsScene *scene, PolygonVariant polygon);

protected:
    void draw() override;
    void onSpacePressed() override;  // Переопределяем для регенерации

private slots:
    void onMouseMoved(QPointF scenePos);

private:
    void updateIndicator(QPointF pos, bool isInside);
    void drawVertexLabels();

    PolygonVariant polygon_;
    QGraphicsTextItem *indicator_;
};

#endif
