#ifndef VISUALIZATIONWIDGET_H
#define VISUALIZATIONWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGraphicsScene>
#include "NavigationView.h"

class VisualizationWidget : public QWidget {
	Q_OBJECT

public:
	explicit VisualizationWidget(QWidget *parent = nullptr);
	virtual ~VisualizationWidget() = default;

	NavigationView* view() { return view_; }
	QGraphicsScene* scene() { return scene_; }

	void addBackButton();
	void addCustomButton(QPushButton* btn);
	void finalizeSetup();

	signals:
		void backPressed();

protected:
	void setupScene();

	NavigationView  *view_;
	QGraphicsScene  *scene_;
	QVBoxLayout     *mainLayout_;
	QHBoxLayout     *buttonLayout_;
};

#endif