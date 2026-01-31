#ifndef TRIANGULATIONCONTROLLER_H
#define TRIANGULATIONCONTROLLER_H

#include "BaseController.h"

class TriangulationController : public BaseController {
    Q_OBJECT

public:
    TriangulationController(NavigationView *view, QGraphicsScene *scene,
                            const std::vector<Point2D>& vertices,
                            const std::vector<Triangle>& triangles);

protected:
    void draw() override;
    void onSpacePressed() override {}

private:
    std::vector<Point2D> vertices_;
    std::vector<Triangle> triangles_;
};

#endif
