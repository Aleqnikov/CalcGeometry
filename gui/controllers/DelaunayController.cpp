#include "DelaunayController.h"

DelaunayController::DelaunayController(NavigationView *view,
                               QGraphicsScene *scene,
                               const std::vector<Point2D>& vertices,
                               const std::vector<Triangle>& triangles,
                               int vertexCount)
    : BaseController(view, scene), vertices_(vertices), triangles_(triangles), vertexCount_(vertexCount)
{
    // Строим Вороного сразу
    DelaunayTriangulation dt;




    for (const auto& p : vertices) dt.insert(p);
    voronoi_ = voronoi_from_delaunay(dt.dcel());

    draw();
}

void DelaunayController::onSpacePressed()
{
    // Если точки были сгенерированы (а не загружены из файла), перегенерируем их
    if (vertexCount_ > 0) {
        regenerate();
    }
}

void DelaunayController::regenerate()
{
    // 1. Генерируем новый набор точек
    Polygon poly = CalcGeometryApi::CreatrePolygon(vertexCount_);
    vertices_ = poly.vertices; // Здесь всё правильно

    // 2. Пересчитываем триангуляцию Делоне
    triangles_ = CalcGeometryApi::Delane(vertices_);

    // 3. Пересчитываем диаграмму Вороного
    DelaunayTriangulation dt;
    for (const auto& p : vertices_) { // <- ИСПРАВЛЕНО: добавлено подчёркивание
        dt.insert(p);
    }
    voronoi_ = voronoi_from_delaunay(dt.dcel());

    bool wasVoronoi = showVoronoi_;
    showVoronoi_ = false;
    draw();
    fitViewToContent();      // кадр по Делоне
    showVoronoi_ = wasVoronoi;
    draw();                  // вернули текущий режим, кадр не трогаем
}

void DelaunayController::toggleMode()
{
    showVoronoi_ = !showVoronoi_;
    draw();
}

void DelaunayController::draw()
{
    clearScene();
    if (showVoronoi_)
       drawVoronoi();
    else
       drawDelaunay();
}

void DelaunayController::drawDelaunay()
{
    QPen   triPen   = createPen(Colors::ACCENT_BLUE, 1);
    QBrush triBrush = createBrush(Colors::ACCENT_BLUE, 40);

    for (const auto& t : triangles_)
       drawPolygon({t.p1, t.p2, t.p3}, triPen, triBrush);

    drawPoints(vertices_, Colors::POINTS_ORANGE, 0.15);
}

void DelaunayController::drawVoronoi()
{
    QPen edgePen = createPen(Colors::PRIMARY_GREEN, 1);
    QPen rayPen  = createPen(Colors::SECONDARY_RED, 1, Qt::DashLine);

    for (const auto& e : voronoi_.edges) {
       if (e.p1) {
          scene_->addLine(e.p0.x, e.p0.y, e.p1->x, e.p1->y, edgePen);
       } else if (e.direction) {
          double ex = e.p0.x + e.direction->x * RAY_LENGTH;
          double ey = e.p0.y + e.direction->y * RAY_LENGTH;
          scene_->addLine(e.p0.x, e.p0.y, ex, ey, rayPen);
       }
    }

    drawPoints(vertices_, Colors::POINTS_ORANGE, 0.15);
}