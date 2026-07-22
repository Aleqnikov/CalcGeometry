#ifndef DELAUNAYCONTROLLER_H
#define DELAUNAYCONTROLLER_H

#include "BaseController.h"
#include "../lib/src/geometry/delone/algorithm/VoronoiFromDelaunay.h"
#include "../lib/src/geometry/delone/algorithm/DelaunayTriangulation.h"

class DelaunayController : public BaseController {
    Q_OBJECT

public:
    // Добавили vertexCount в конструктор (0 по умолчанию, если загружено из JSON)
    DelaunayController(NavigationView *view, QGraphicsScene *scene,
                       const std::vector<Point2D>& vertices,
                       const std::vector<Triangle>& triangles,
                       int vertexCount = 0);

    void toggleMode();
    bool isShowingVoronoi() const { return showVoronoi_; }

protected:
    void draw() override;
    void onSpacePressed() override; // Переопределяем метод для обработки пробела

private:
    void drawDelaunay();
    void drawVoronoi();
    void regenerate(); // Вспомогательный метод перегенерации

    std::vector<Point2D>  vertices_;
    std::vector<Triangle> triangles_;
    VoronoiDiagram        voronoi_;
    int                   vertexCount_ = 0; // Сохраняем количество для перегенерации
    bool                  showVoronoi_ = false;

    static constexpr double RAY_LENGTH = 500.0;
};

#endif