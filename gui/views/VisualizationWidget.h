#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsScene>
#include "NavigationView.h"

class VisualizationWidget : public QWidget {
    Q_OBJECT

public:
    explicit VisualizationWidget(QWidget *parent = nullptr);
    virtual ~VisualizationWidget() = default;

    // Геттеры для доступа к view и scene
    NavigationView* view() { return view_; }
    QGraphicsScene* scene() { return scene_; }

    // Публичные методы настройки (вызываются из MainWindow)
    void addBackButton();
    void finalizeSetup();

protected:
    void setupScene();  // Только это остается protected

    NavigationView *view_;
    QGraphicsScene *scene_;
    QVBoxLayout *mainLayout_;

signals:
    void backPressed();
};

#endif
