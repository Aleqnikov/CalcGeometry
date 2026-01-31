#include "MainWindow.h"
#include "dialogs/PolygonSettingsDialog.h"
#include "dialogs/HullSettingsDialog.h"
#include "dialogs/TriangulationSettingsDialog.h"
#include "dialogs/DiameterSettingsDialog.h"
#include "dialogs/RectangleSettingsDialog.h"
#include "dialogs/CircleSettingsDialog.h"
#include "views/VisualizationWidget.h"
#include "controllers/PolygonController.h"
#include "controllers/HullController.h"
#include "controllers/TriangulationController.h"
#include "controllers/DiameterController.h"
#include "controllers/RectangleController.h"
#include "controllers/CircleController.h"
#include "jsonparcer.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    stack_ = new QStackedWidget(this);
    setCentralWidget(stack_);
    setMinimumSize(1000, 700);
    setWindowTitle(tr("Вычислительная Геометрия"));

    createStartMenu();
}

void MainWindow::createStartMenu()
{
    QWidget *container = new QWidget(stack_);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(80, 60, 80, 60);
    mainLayout->setSpacing(30);

    QLabel *titleLabel = new QLabel(tr("Вычислительная геометрия"), container);
    mainLayout->addWidget(titleLabel);
    mainLayout->addStretch();

    // Структура для упрощения создания кнопок меню
    struct MenuItem {
        QString text;
        void (MainWindow::*slot)();
    };

    const QList<MenuItem> menuItems = {
        {tr("Точка внутри полигона"), &MainWindow::showPolygonSettings},
        {tr("Построение выпуклой оболочки"), &MainWindow::showHullSettings},
        {tr("Триангуляция"), &MainWindow::showTriangulationSettings},
        {tr("Диаметр"), &MainWindow::showDiameterSettings},
        {tr("Прямоугольник"), &MainWindow::showRectangleSettings},
        {tr("Круг"), &MainWindow::showCircleSettings}
    };

    for (const auto& item : menuItems) {
        auto *btn = new QPushButton(item.text, container);
        connect(btn, &QPushButton::clicked, this, item.slot);
        mainLayout->addWidget(btn);
    }

    container->setLayout(mainLayout);
    stack_->insertWidget(static_cast<int>(PageIndex::MainMenu), container);
}

void MainWindow::showSettings(QWidget* settingsDialog)
{
    stack_->insertWidget(static_cast<int>(PageIndex::Settings), settingsDialog);
    stack_->setCurrentIndex(static_cast<int>(PageIndex::Settings));
}

void MainWindow::returnToMainMenu()
{
    stack_->setCurrentIndex(static_cast<int>(PageIndex::MainMenu));
}

// === SETTINGS SLOTS ===

void MainWindow::showPolygonSettings()
{
    auto *dialog = new PolygonSettingsDialog();

    connect(dialog, &SettingsDialog::confirmed, this, [this, dialog]() {
        const int n = dialog->getVertexCount();
        const QString type = dialog->getPolygonType();

        PolygonVariant polygon = CalcGeometryApi::CreatrePolygon(n);

        if (type == tr("Выпуклый"))
            polygon = CalcGeometryApi::CreatreConvexPolygon(n);
        else if (type == tr("Звёздчатый"))
            polygon = CalcGeometryApi::CreatreStarPolygon(n);

        showPolygonVisualization(polygon);
    });

    showSettings(dialog);
}

void MainWindow::showHullSettings()
{
    auto *dialog = new HullSettingsDialog();

    connect(dialog, &SettingsDialog::confirmed, this, [this, dialog]() {
        const int n = dialog->getVertexCount();
        const bool useAndrew = dialog->useAndrewAlgorithm();

        Hull hull = CalcGeometryApi::CreateHull(n);
        showHullVisualization(hull, useAndrew);
    });

    showSettings(dialog);
}

void MainWindow::showTriangulationSettings()
{
    auto *dialog = new TriangulationSettingsDialog();

    connect(dialog, &SettingsDialog::confirmed, this, [this, dialog]() {
        std::vector<Point2D> vertices;

        if (dialog->shouldLoadFromJSON()) {
            vertices = loadVerticesFromJSON();
            if (vertices.empty()) return;
        } else {
            const int n = dialog->getVertexCount();
            Polygon poly = CalcGeometryApi::CreatrePolygon(n);
            vertices = poly.vertices;
        }

        if (!vertices.empty()) {
            auto triangles = CalcGeometryApi::EarClipping(vertices);
            showTriangulationVisualization(vertices, triangles);
        }
    });

    showSettings(dialog);
}

void MainWindow::showDiameterSettings()
{
    auto *dialog = new DiameterSettingsDialog();

    connect(dialog, &SettingsDialog::confirmed, this, [this, dialog]() {
        const int n = dialog->getPointCount();
        const auto mode = dialog->getMode();
        showDiameterVisualization(n, mode);
    });

    showSettings(dialog);
}

void MainWindow::showRectangleSettings()
{
    auto *dialog = new RectangleSettingsDialog();

    connect(dialog, &SettingsDialog::confirmed, this, [this, dialog]() {
        const int n = dialog->getPointCount();
        showRectangleVisualization(n);
    });

    showSettings(dialog);
}

void MainWindow::showCircleSettings()
{
    auto *dialog = new CircleSettingsDialog();

    connect(dialog, &SettingsDialog::confirmed, this, [this, dialog]() {
        const int n = dialog->getPointCount();
        const auto mode = dialog->getMode();
        showCircleVisualization(n, mode);
    });

    showSettings(dialog);
}

// === VISUALIZATION SLOTS ===

void MainWindow::showPolygonVisualization(const PolygonVariant& polygon)
{
    auto *widget = new VisualizationWidget();
    new PolygonController(widget->view(), widget->scene(), polygon);
    widget->addBackButton();
    widget->finalizeSetup();

    connect(widget, &VisualizationWidget::backPressed, this, &MainWindow::returnToMainMenu);

    stack_->insertWidget(static_cast<int>(PageIndex::Visualization), widget);
    stack_->setCurrentIndex(static_cast<int>(PageIndex::Visualization));
}

void MainWindow::showHullVisualization(Hull hull, bool useAndrew)
{
    auto *widget = new VisualizationWidget();
    new HullController(widget->view(), widget->scene(), std::move(hull), useAndrew);
    widget->addBackButton();
    widget->finalizeSetup();

    connect(widget, &VisualizationWidget::backPressed, this, &MainWindow::returnToMainMenu);

    stack_->insertWidget(static_cast<int>(PageIndex::Visualization), widget);
    stack_->setCurrentIndex(static_cast<int>(PageIndex::Visualization));
}

void MainWindow::showTriangulationVisualization(const std::vector<Point2D>& vertices,
                                                const std::vector<Triangle>& triangles)
{
    auto *widget = new VisualizationWidget();
    new TriangulationController(widget->view(), widget->scene(), vertices, triangles);
    widget->addBackButton();
    widget->finalizeSetup();

    connect(widget, &VisualizationWidget::backPressed, this, &MainWindow::returnToMainMenu);

    stack_->insertWidget(static_cast<int>(PageIndex::Visualization), widget);
    stack_->setCurrentIndex(static_cast<int>(PageIndex::Visualization));
}

void MainWindow::showDiameterVisualization(int n, int mode)
{
    auto *widget = new VisualizationWidget();
    new DiameterController(widget->view(), widget->scene(), n,
                           static_cast<DiameterController::Mode>(mode));
    widget->addBackButton();
    widget->finalizeSetup();

    connect(widget, &VisualizationWidget::backPressed, this, &MainWindow::returnToMainMenu);

    stack_->insertWidget(static_cast<int>(PageIndex::Visualization), widget);
    stack_->setCurrentIndex(static_cast<int>(PageIndex::Visualization));
}

void MainWindow::showRectangleVisualization(int n)
{
    auto *widget = new VisualizationWidget();
    new RectangleController(widget->view(), widget->scene(), n);
    widget->addBackButton();
    widget->finalizeSetup();

    connect(widget, &VisualizationWidget::backPressed, this, &MainWindow::returnToMainMenu);

    stack_->insertWidget(static_cast<int>(PageIndex::Visualization), widget);
    stack_->setCurrentIndex(static_cast<int>(PageIndex::Visualization));
}

void MainWindow::showCircleVisualization(int n, int mode)
{
    auto *widget = new VisualizationWidget();
    new CircleController(widget->view(), widget->scene(), n,
                         static_cast<CircleController::Mode>(mode));
    widget->addBackButton();
    widget->finalizeSetup();

    connect(widget, &VisualizationWidget::backPressed, this, &MainWindow::returnToMainMenu);

    stack_->insertWidget(static_cast<int>(PageIndex::Visualization), widget);
    stack_->setCurrentIndex(static_cast<int>(PageIndex::Visualization));
}

// === HELPER METHODS ===

std::vector<Point2D> MainWindow::loadVerticesFromJSON()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Открыть JSON"), "", "*.json");
    if (file.isEmpty()) return {};

    QList<QPointF> qpoints = ContourJsonLoader::load(file);

    if (qpoints.size() < 3) {
        QMessageBox::warning(this, tr("Ошибка"), tr("В файле слишком мало точек!"));
        return {};
    }

    std::vector<Point2D> vertices;
    vertices.reserve(qpoints.size());
    for (const auto &p : qpoints) {
        vertices.emplace_back(p.x(), p.y());
    }

    return vertices;
}
