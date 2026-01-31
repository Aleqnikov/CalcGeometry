#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <vector>
#include "../lib/api/GeometryApi.h"

using PolygonVariant = std::variant<Polygon, ConvexPolygon, StarPolygon>;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    // Settings screens
    void showPolygonSettings();
    void showHullSettings();
    void showTriangulationSettings();
    void showDiameterSettings();
    void showRectangleSettings();
    void showCircleSettings();

    // Visualization screens
    void showPolygonVisualization(const PolygonVariant& polygon);
    void showHullVisualization(Hull hull, bool useAndrew);
    void showTriangulationVisualization(const std::vector<Point2D>& vertices,
                                        const std::vector<Triangle>& triangles);
    void showDiameterVisualization(int n, int mode);
    void showRectangleVisualization(int n);
    void showCircleVisualization(int n, int mode);

    void returnToMainMenu();

private:
    void createStartMenu();
    void showSettings(QWidget* settingsDialog);
    std::vector<Point2D> loadVerticesFromJSON();

    enum class PageIndex { MainMenu = 0, Settings = 1, Visualization = 2 };

    QStackedWidget *stack_;
};

#endif
