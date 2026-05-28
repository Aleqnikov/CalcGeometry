#include "VisualizationWidget.h"
#include <QTimer>

VisualizationWidget::VisualizationWidget(QWidget *parent)
	: QWidget(parent)
{
	mainLayout_   = new QVBoxLayout(this);
	buttonLayout_ = new QHBoxLayout();
	mainLayout_->setContentsMargins(0, 0, 0, 0);

	scene_ = new QGraphicsScene(this);
	view_  = new NavigationView(scene_, this);

	setupScene();

	// buttonLayout_ добавляем в конец — кнопки будут снизу
	mainLayout_->addLayout(buttonLayout_);
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
	buttonLayout_->addWidget(backBtn);
}

void VisualizationWidget::addCustomButton(QPushButton* btn)
{
	btn->setParent(this);
	buttonLayout_->addWidget(btn);
}

void VisualizationWidget::finalizeSetup()
{
	QTimer::singleShot(0, this, [this]() {
		if (!scene_->items().isEmpty())
			view_->fitInView(scene_->itemsBoundingRect(), Qt::KeepAspectRatio);
		view_->setFocus();
	});
}