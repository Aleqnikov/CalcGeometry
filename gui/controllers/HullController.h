#ifndef HULLCONTROLLER_H
#define HULLCONTROLLER_H

#include "BaseController.h"

class HullController : public BaseController {
    Q_OBJECT

public:
    HullController(NavigationView *view, QGraphicsScene *scene, Hull hull, bool useAndrew);

protected:
    void draw() override;
    void onSpacePressed() override;  // Переопределяем для регенерации

private:
    Hull hullModel_;
    bool useAndrew_;
};

#endif
