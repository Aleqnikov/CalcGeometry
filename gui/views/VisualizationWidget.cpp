#include "VisualizationWidget.h"
#include <QTimer>  // <-- ДОБАВЬТЕ ЭТО


VisualizationWidget::VisualizationWidget(QWidget *parent)
    : QWidget(parent)
{
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(0, 0, 0, 0);

    scene_ = new QGraphicsScene(this);
    view_ = new NavigationView(scene_, this);

    setupScene();
}

void VisualizationWidget::setupScene()
{
    view_->setSceneRect(-1000000, -1000000, 2000000, 2000000);
    view_->scale(1, -1);
    mainLayout_->addWidget(view_);
}

void VisualizationWidget::addBackButton()
{
    QPushButton *backBtn = new QPushButton(tr("Назад"), this);
    backBtn->setCursor(Qt::PointingHandCursor);
    connect(backBtn, &QPushButton::clicked, this, &VisualizationWidget::backPressed);
    mainLayout_->addWidget(backBtn);
}

void VisualizationWidget::finalizeSetup()
{
    // ИСПРАВЛЕНИЕ: Добавляем небольшую задержку, чтобы сцена успела отрисоваться
    QTimer::singleShot(0, this, [this]() {
        if (!scene_->items().isEmpty()) {
            view_->fitInView(scene_->itemsBoundingRect(), Qt::KeepAspectRatio);
        }
        view_->setFocus();
    });
}
