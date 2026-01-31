#ifndef DIAMETERCONTROLLER_H
#define DIAMETERCONTROLLER_H

#include "BaseController.h"

class DiameterController : public BaseController {
    Q_OBJECT

public:
    enum Mode { Naive, Calipers, CompareBoth };

    DiameterController(NavigationView *view, QGraphicsScene *scene, int n, Mode mode);

protected:
    void draw() override;

private:
    int n_;
    Mode mode_;
};

#endif
