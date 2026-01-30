#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStackedWidget>


#include "../lib/api/GeometryApi.h"
#include "DiameterController.h"
#include "CircleController.h"

using PolygonVariant = std::variant<Polygon, ConvexPolygon, StarPolygon>;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private:
    Ui::MainWindow *ui;

    QStackedWidget* stack;

    void createStartMenu();

    void PolygonSettings();
    void ConvexHullSettings();
    void TriangulationSettings();
    void DiameterSettings();
    void RectangleSettings();
    void CircleSettings();

    void showPolygonVisualization(const PolygonVariant& polygon);
    void showHullVisualization(Hull mesh, bool mod);
    void showTriangulationVisualization(const std::vector<Point2D>& vertices,
                                        const std::vector<Triangle>& triangles);




    void showDiameterVisualization(int n, DiameterController::Mode mode);
    void showRectangleVisualization(int n_);

    void showCircleVisualization(int n, CircleController::Mode mode);

};
#endif // MAINWINDOW_H
