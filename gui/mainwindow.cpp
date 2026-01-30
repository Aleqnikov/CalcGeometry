#include "mainwindow.h"

#include "NavigationView.h"
#include "PolygonController.h"
#include "HullController.h"
#include "jsonparcer.h"
#include "TriangulationController.h"
#include "RectangleController.h"


#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    stack = new QStackedWidget(this);
    setCentralWidget(stack);
    setMinimumSize(1000, 700);
    setWindowTitle(tr("Вычислительная Геометрия"));

    createStartMenu();
}


void MainWindow::createStartMenu()
{
    QWidget *container = new QWidget(stack);

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->setContentsMargins(80, 60, 80, 60);
    mainLayout->setSpacing(30);

    QLabel *titleLabel = new QLabel(tr("Вычислительная геометрия"), container);

    mainLayout->addWidget(titleLabel);
    mainLayout->addStretch();

    auto *btnPol = new QPushButton("Точка внутри полигона");
    connect(btnPol, &QPushButton::clicked, this, &MainWindow::PolygonSettings);

    auto *btnConHul = new QPushButton("Построение выпуклой оболочки");
    connect(btnConHul, &QPushButton::clicked, this, &MainWindow::ConvexHullSettings);

    auto *btnTriang = new QPushButton("Триангуляция");
    connect(btnTriang, &QPushButton::clicked, this, &MainWindow::TriangulationSettings);

    auto *btnDiam = new QPushButton("Диаметр");
    connect(btnDiam, &QPushButton::clicked, this, &MainWindow::DiameterSettings);

    auto *btnRect= new QPushButton("Прямоугольник");
    connect(btnRect, &QPushButton::clicked, this, &MainWindow::RectangleSettings);

    auto *btnCirc= new QPushButton("Круг");
    connect(btnCirc, &QPushButton::clicked, this, &MainWindow::CircleSettings);

    mainLayout->addWidget(btnPol);
    mainLayout->addWidget(btnConHul);
    mainLayout->addWidget(btnTriang);
    mainLayout->addWidget(btnDiam);
    mainLayout->addWidget(btnRect);
    mainLayout->addWidget(btnCirc);


    container->setLayout(mainLayout);
    stack->insertWidget(0, container);
}


void MainWindow::PolygonSettings()
{
    QWidget *page = new QWidget();

    QVBoxLayout *mainLayout = new QVBoxLayout(page);


    QLabel *titleLabel = new QLabel(tr("Точка внутри многоугольника"), page);

    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addStretch();

    QGroupBox *paramGroup = new QGroupBox(tr("Параметры"), page);


    QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);

    QLabel *countLabel = new QLabel(tr("Количество вершин:"), paramGroup);
    groupLayout->addWidget(countLabel);

    QSpinBox *spin = new QSpinBox(paramGroup);
    spin->setRange(3, 10000);
    spin->setValue(10);

    groupLayout->addWidget(spin);

    QLabel *modeLabel = new QLabel(tr("Тип многоугольника:"), paramGroup);
    groupLayout->addWidget(modeLabel);

    QComboBox *combo = new QComboBox(paramGroup);
    combo->addItems({tr("Выпуклый"), tr("Звёздчатый"), tr("Обычный")});

    groupLayout->addWidget(combo);

    paramGroup->setLayout(groupLayout);
    mainLayout->addWidget(paramGroup);

    mainLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();


    QPushButton *confirm = new QPushButton(tr("Создать"), page);

    confirm->setCursor(Qt::PointingHandCursor);

    buttonLayout->addStretch();
    buttonLayout->addWidget(confirm);

    mainLayout->addLayout(buttonLayout);

    page->setLayout(mainLayout);
    stack->insertWidget(1, page);
    stack->setCurrentIndex(1);

    connect(confirm, &QPushButton::clicked, this, [this, spin, combo]() {
        const int n = spin->value();

        // Инициализируем пустым состоянием
        PolygonVariant polygon = CalcGeometryApi::CreatrePolygon(n);

        const QString choice = combo->currentText();
        if (choice == tr("Выпуклый"))
            polygon = CalcGeometryApi::CreatreConvexPolygon(n);
        else if (choice == tr("Звёздчатый"))
            polygon = CalcGeometryApi::CreatreStarPolygon(n);


        showPolygonVisualization(polygon);
    });


}

void MainWindow::ConvexHullSettings()
{
    QWidget *page = new QWidget();

    QVBoxLayout *mainLayout = new QVBoxLayout(page);


    QLabel *titleLabel = new QLabel(tr("Построение выпуклой оболочки"), page);

    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addStretch();

    QGroupBox *paramGroup = new QGroupBox(tr("Параметры"), page);


    QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);

    QLabel *countLabel = new QLabel(tr("Количество вершин:"), paramGroup);
    groupLayout->addWidget(countLabel);

    QSpinBox *spin = new QSpinBox(paramGroup);
    spin->setRange(3, 10000);
    spin->setValue(10);

    groupLayout->addWidget(spin);

    QLabel *modeLabel = new QLabel(tr("Алгоритм"), paramGroup);
    groupLayout->addWidget(modeLabel);

    QComboBox *combo = new QComboBox(paramGroup);
    combo->addItems({tr("Джарвис"), tr("Андрюс")});

    groupLayout->addWidget(combo);

    paramGroup->setLayout(groupLayout);
    mainLayout->addWidget(paramGroup);

    mainLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();


    QPushButton *confirm = new QPushButton(tr("Создать"), page);

    confirm->setCursor(Qt::PointingHandCursor);

    buttonLayout->addStretch();
    buttonLayout->addWidget(confirm);

    mainLayout->addLayout(buttonLayout);

    page->setLayout(mainLayout);
    stack->insertWidget(1, page);
    stack->setCurrentIndex(1);

    connect(confirm, &QPushButton::clicked, this, [this, spin, combo]() {
        const int n = spin->value();

        // Инициализируем пустым состоянием
        Hull mesh = CalcGeometryApi::CreateHull(n);


        bool mod = true;

        const QString choice = combo->currentText();

        if (choice == tr("Джарвис"))
            mod = false;


        showHullVisualization(mesh, mod);
    });
}

void MainWindow::TriangulationSettings()
{
    QWidget *page = new QWidget();

    QVBoxLayout *mainLayout = new QVBoxLayout(page);

    QLabel *titleLabel = new QLabel(tr("Триангуляция полигона"), page);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addStretch();

    QGroupBox *paramGroup = new QGroupBox(tr("Параметры"), page);

    QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);

    QLabel *countLabel = new QLabel(tr("Количество вершин:"), paramGroup);
    groupLayout->addWidget(countLabel);

    QSpinBox *spin = new QSpinBox(paramGroup);
    spin->setRange(3, 10000);
    spin->setValue(12);
    groupLayout->addWidget(spin);

    QLabel *modeLabel = new QLabel(tr("Метод или источник:"), paramGroup);
    groupLayout->addWidget(modeLabel);

    QComboBox *combo = new QComboBox(paramGroup);
    combo->addItems({tr("Сгенерировать"), tr("Загрузить JSON")});
    groupLayout->addWidget(combo);

    paramGroup->setLayout(groupLayout);
    mainLayout->addWidget(paramGroup);

    mainLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *confirm = new QPushButton(tr("Создать"), page);
    confirm->setCursor(Qt::PointingHandCursor);

    buttonLayout->addStretch();
    buttonLayout->addWidget(confirm);

    mainLayout->addLayout(buttonLayout);

    page->setLayout(mainLayout);
    stack->insertWidget(1, page);
    stack->setCurrentIndex(1);

    connect(confirm, &QPushButton::clicked, this, [this, spin, combo]() {
        const int n = spin->value();
        const QString choice = combo->currentText();

        std::vector<Point2D> vertices;

        if (choice == tr("Сгенерировать")) {
            Polygon poly = CalcGeometryApi::CreatrePolygon(n);
            vertices = poly.vertices;
        }
        else if (choice == tr("Загрузить JSON")) {
            QString file = QFileDialog::getOpenFileName(this, tr("Открыть JSON"), "", "*.json");
            if (file.isEmpty()) return;

            QList<QPointF> qpoints = ContourJsonLoader::load(file);

            if (qpoints.size() < 3) {
                QMessageBox::warning(this, tr("Ошибка"), tr("В файле слишком мало точек!"));
                return;
            }

            for (const auto &p : qpoints) {
                vertices.emplace_back(p.x(), p.y());
            }
        }

        if (!vertices.empty()) {
            auto triangles = CalcGeometryApi::EarClipping(vertices);
            showTriangulationVisualization(vertices, triangles);
        }
    });
}

void MainWindow::DiameterSettings()
{
    QWidget *page = new QWidget();

    QVBoxLayout *mainLayout = new QVBoxLayout(page);


    QLabel *titleLabel = new QLabel(tr("Нахождение диаметра множества"), page);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addStretch();

    QGroupBox *paramGroup = new QGroupBox(tr("Параметры"), page);
    QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);

    QLabel *countLabel = new QLabel(tr("Количество точек:"), paramGroup);
    groupLayout->addWidget(countLabel);

    QSpinBox *spin = new QSpinBox(paramGroup);
    spin->setRange(2, 10000);
    spin->setValue(200);
    groupLayout->addWidget(spin);

    QLabel *modeLabel = new QLabel(tr("Алгоритм:"), paramGroup);
    groupLayout->addWidget(modeLabel);

    QComboBox *combo = new QComboBox(paramGroup);
    combo->addItems({
        tr("Наивный (O(n^2))"),
        tr("Rotating Calipers"),
        tr("Сравнение (оба)")
    });
    groupLayout->addWidget(combo);

    paramGroup->setLayout(groupLayout);
    mainLayout->addWidget(paramGroup);

    mainLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(20);

    QPushButton *confirm = new QPushButton(tr("Создать"), page);
    confirm->setCursor(Qt::PointingHandCursor);

    buttonLayout->addStretch();
    buttonLayout->addWidget(confirm);

    mainLayout->addLayout(buttonLayout);

    page->setLayout(mainLayout);
    stack->insertWidget(1, page);
    stack->setCurrentIndex(1);

    connect(confirm, &QPushButton::clicked, this, [this, spin, combo]() {
        const int n = spin->value();
        const QString choice = combo->currentText();

        DiameterController::Mode mode;

        if (choice == tr("Наивный (O(n^2))")) {
            mode = DiameterController::Naive;
        } else if (choice == tr("Rotating Calipers (требует оболочку)")) {
            mode = DiameterController::Calipers;
        } else {
            mode = DiameterController::CompareBoth; // Для "Полный" или "Сравнение"
        }

        showDiameterVisualization(n, mode);
    });
}

void MainWindow::RectangleSettings()
{
    QWidget *page = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(page);


    QLabel *titleLabel = new QLabel(tr("Минимальный ограничивающий прямоугольник"), page);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addStretch();

    QGroupBox *paramGroup = new QGroupBox(tr("Параметры"), page);
    QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);

    QLabel *countLabel = new QLabel(tr("Количество точек:"), paramGroup);
    groupLayout->addWidget(countLabel);

    QSpinBox *spin = new QSpinBox(paramGroup);
    spin->setRange(3, 10000);
    spin->setValue(100);
    groupLayout->addWidget(spin);

    paramGroup->setLayout(groupLayout);
    mainLayout->addWidget(paramGroup);

    mainLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *confirm = new QPushButton(tr("Создать"), page);
    confirm->setCursor(Qt::PointingHandCursor);
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirm);
    mainLayout->addLayout(buttonLayout);

    page->setLayout(mainLayout);
    stack->insertWidget(1, page);
    stack->setCurrentIndex(1);

    connect(confirm, &QPushButton::clicked, this, [this, spin]() {
        const int n = spin->value();
        showRectangleVisualization(n);
    });
}

void MainWindow::CircleSettings()
{
    QWidget *page = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    mainLayout->setContentsMargins(80, 60, 80, 60);
    mainLayout->setSpacing(25);

    QLabel *titleLabel = new QLabel(tr("Минимальная ограничивающая окружность"), page);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addStretch();

    QGroupBox *paramGroup = new QGroupBox(tr("Параметры"), page);
    QVBoxLayout *groupLayout = new QVBoxLayout(paramGroup);
    groupLayout->setSpacing(18);

    QLabel *countLabel = new QLabel(tr("Количество точек:"), paramGroup);
    groupLayout->addWidget(countLabel);

    QSpinBox *spin = new QSpinBox(paramGroup);
    spin->setRange(1, 10000);
    spin->setValue(200);
    groupLayout->addWidget(spin);

    QLabel *modeLabel = new QLabel(tr("Алгоритм:"), paramGroup);
    groupLayout->addWidget(modeLabel);

    QComboBox *combo = new QComboBox(paramGroup);
    combo->addItems({ tr("Наивный (перебор)"), tr("Welzl (рандомизированный)"), tr("Сравнить оба") });
    groupLayout->addWidget(combo);

    paramGroup->setLayout(groupLayout);
    mainLayout->addWidget(paramGroup);

    mainLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *confirm = new QPushButton(tr("Создать"), page);
    confirm->setCursor(Qt::PointingHandCursor);
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirm);
    mainLayout->addLayout(buttonLayout);

    page->setLayout(mainLayout);
    stack->insertWidget(1, page);
    stack->setCurrentIndex(1);

    connect(confirm, &QPushButton::clicked, this, [this, spin, combo]() {
        const int n = spin->value();
        const QString choice = combo->currentText();

        CircleController::Mode mode;
        if (choice.contains("Наивный")) mode = CircleController::Naive;
        else if (choice.contains("Welzl")) mode = CircleController::Welzl;
        else mode = CircleController::CompareBoth;

        showCircleVisualization(n, mode);
    });
}

void MainWindow::showPolygonVisualization(const PolygonVariant& polygon)
{
    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    QGraphicsScene *scene = new QGraphicsScene();
    NavigationView *view = new NavigationView(scene);
    view->setSceneRect(-1000000, -1000000, 2000000, 2000000);
    view->scale(1, -1);

    new PolygonController(view, scene, polygon);

    layout->addWidget(view);

    // Кнопка назад
    QPushButton *backBtn = new QPushButton(tr("Назад"), container);
    backBtn->setCursor(Qt::PointingHandCursor);
    connect(backBtn, &QPushButton::clicked, this, [this](){ stack->setCurrentIndex(0); });
    layout->addWidget(backBtn);

    stack->insertWidget(2, container);
    stack->setCurrentIndex(2);

    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    view->setFocus();
}


void MainWindow::showHullVisualization(Hull mesh, bool mod)
{
    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    QGraphicsScene *scene = new QGraphicsScene();
    NavigationView *view = new NavigationView(scene);

    // Настраиваем бесконечную сцену
    view->setSceneRect(-1000000, -1000000, 2000000, 2000000);
    // Инвертируем Y, чтобы координаты шли вверх, как в математике
    view->scale(1, -1);

    // Создаем наш новый контроллер для оболочки
    // Он сам все нарисует при создании
    new HullController(view, scene, std::move(mesh), mod);

    layout->addWidget(view);

    // Кнопка назад
    QPushButton *backBtn = new QPushButton(tr("Назад"), container);
    backBtn->setCursor(Qt::PointingHandCursor);
    connect(backBtn, &QPushButton::clicked, this, [this](){ stack->setCurrentIndex(0); });
    layout->addWidget(backBtn);

    stack->insertWidget(2, container);
    stack->setCurrentIndex(2);

    // Центрируем камеру на точках
    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    view->setFocus();
}


void MainWindow::showTriangulationVisualization(const std::vector<Point2D>& vertices,
                                                const std::vector<Triangle>& triangles)
{
    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    QGraphicsScene *scene = new QGraphicsScene();
    NavigationView *view = new NavigationView(scene);
    view->setSceneRect(-1000000, -1000000, 2000000, 2000000);
    view->scale(1, -1);

    // Контроллер сам все отрисует
    new TriangulationController(view, scene, vertices, triangles);

    layout->addWidget(view);

    // Кнопка назад в твоем стиле
    QPushButton *backBtn = new QPushButton(tr("Назад"), container);
    backBtn->setCursor(Qt::PointingHandCursor);
    connect(backBtn, &QPushButton::clicked, this, [this](){ stack->setCurrentIndex(0); });
    layout->addWidget(backBtn);

    stack->insertWidget(2, container);
    stack->setCurrentIndex(2);

    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    view->setFocus();
}


void MainWindow::showDiameterVisualization(int n, DiameterController::Mode mode)
{
    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    QGraphicsScene *scene = new QGraphicsScene();
    NavigationView *view = new NavigationView(scene);
    view->setSceneRect(-1000000, -1000000, 2000000, 2000000);
    view->scale(1, -1);

    // Передаем выбранный режим в контроллер
    new DiameterController(view, scene, n, mode);

    layout->addWidget(view);

    QPushButton *backBtn = new QPushButton(tr("Назад"), container);
    backBtn->setCursor(Qt::PointingHandCursor);
    connect(backBtn, &QPushButton::clicked, this, [this](){ stack->setCurrentIndex(0); });
    layout->addWidget(backBtn);

    stack->insertWidget(2, container);
    stack->setCurrentIndex(2);

    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    view->setFocus();
}

void MainWindow::showRectangleVisualization(int n)
{
    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    QGraphicsScene *scene = new QGraphicsScene();
    NavigationView *view = new NavigationView(scene);
    view->setSceneRect(-1000000, -1000000, 2000000, 2000000);
    view->scale(1, -1);

    // Создаем контроллер, передаем количество точек
    new RectangleController(view, scene, n);

    layout->addWidget(view);

    QPushButton *backBtn = new QPushButton(tr("Назад"), container);
    backBtn->setCursor(Qt::PointingHandCursor);
    connect(backBtn, &QPushButton::clicked, this, [this](){ stack->setCurrentIndex(0); });
    layout->addWidget(backBtn);

    stack->insertWidget(2, container);
    stack->setCurrentIndex(2);

    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    view->setFocus();
}

void MainWindow::showCircleVisualization(int n, CircleController::Mode mode)
{
    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    QGraphicsScene *scene = new QGraphicsScene();
    NavigationView *view = new NavigationView(scene);
    view->setSceneRect(-1000000, -1000000, 2000000, 2000000);
    view->scale(1, -1);

    // Контроллер запускает всю магию
    new CircleController(view, scene, n, mode);

    layout->addWidget(view);

    QPushButton *backBtn = new QPushButton(tr("Назад"), container);
    backBtn->setCursor(Qt::PointingHandCursor);
    connect(backBtn, &QPushButton::clicked, this, [this](){ stack->setCurrentIndex(0); });
    layout->addWidget(backBtn);

    stack->insertWidget(2, container);
    stack->setCurrentIndex(2);

    view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);
    view->setFocus();
}
