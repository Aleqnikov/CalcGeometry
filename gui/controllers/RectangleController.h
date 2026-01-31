#ifndef RECTANGLECONTROLLER_H
#define RECTANGLECONTROLLER_H

#include "BaseController.h"

class RectangleController : public BaseController {
    Q_OBJECT

public:
    RectangleController(NavigationView *view, QGraphicsScene *scene, int n);

protected:
    void draw() override;

private:
    int n_;
};

#endif
