#ifndef CIRCLECONTROLLER_H
#define CIRCLECONTROLLER_H

#include "BaseController.h"

class CircleController : public BaseController {
    Q_OBJECT

public:
    enum Mode { Naive, Welzl, CompareBoth };

    CircleController(NavigationView *view, QGraphicsScene *scene, int n, Mode mode);

protected:
    void draw() override;

private:
    int n_;
    Mode mode_;
};

#endif
