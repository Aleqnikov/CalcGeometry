# Вычислительная геометрия

Учебный C++/Qt-проект: алгоритмы вычислительной геометрии и их интерактивная визуализация.

Проект разделён на два слоя — графический интерфейс (`gui/`) и алгоритмическая библиотека (`lib/`). GUI зависит от библиотеки, обратной зависимости нет.



## Сборка и запуск

Всё собирается и запускается из `gui/` — это единственная точка входа, библиотека подключается через `gui.pro`.

**Требования:** C++17, Qt 5.15, qmake.

### Qt Creator

Открыть `gui/gui.pro`, выбрать комплект Qt 5.15 и нажать «Запустить».

### Командная строка

```bash
cd gui
mkdir -p build && cd build
qmake ..
make
```

Запуск собранного приложения:

```bash
./gui.app/Contents/MacOS/gui   # macOS
./gui                          # Linux
```


---

## Структура проекта

```
.
├── gui/                        # Qt-приложение (точка сборки и запуска)
│   ├── gui.pro                 # файл проекта qmake
│   ├── main.cpp
│   ├── mainwindow.{h,cpp}      # главное меню и навигация между экранами
│   ├── jsonparcer.h            # загрузка контуров из JSON
│   ├── views/
│   │   ├── NavigationView.h        # QGraphicsView с зумом и панорамированием
│   │   └── VisualizationWidget.{h,cpp}
│   ├── dialogs/                # окна ввода параметров для каждой задачи
│   │   ├── SettingsDialog.{h,cpp}      # базовый диалог
│   │   ├── PolygonSettingsDialog.{h,cpp}
│   │   ├── HullSettingsDialog.{h,cpp}
│   │   ├── TriangulationSettingsDialog.{h,cpp}
│   │   ├── DelaunaySettingsDialog.{h,cpp}
│   │   ├── DiameterSettingsDialog.{h,cpp}
│   │   ├── RectangleSettingsDialog.{h,cpp}
│   │   └── CircleSettingsDialog.{h,cpp}
│   └── controllers/            # связывают алгоритмы из lib/ со сценой
│       ├── BaseController.{h,cpp}      # общая отрисовка, обработка клавиш
│       ├── PolygonController.{h,cpp}
│       ├── HullController.{h,cpp}
│       ├── TriangulationController.{h,cpp}
│       ├── DelaunayController.{h,cpp}  # Делоне / Вороной, переключение режимов
│       ├── DiameterController.{h,cpp}
│       ├── RectangleController.{h,cpp}
│       └── CircleController.{h,cpp}
│
└── lib/                        # алгоритмическая часть, без зависимостей от Qt
    ├── api/
    │   └── GeometryApi.{h,cpp}         # единая точка доступа для GUI
    └── src/
        ├── linalgebra/
        │   ├── Point2D.h
        │   └── LinAl.{h,cpp}           # ориентация, расстояния, произведения
        └── geometry/
            ├── polygons/
            │   ├── core/               # Polygon, ConvexPn, StarPn
            │   └── algorithms/         # point_in_polygon
            ├── convexhull/
            │   ├── core/               # Hull
            │   └── algorithms/         # HullAlgorithms
            ├── triangulation/
            │   └── algorithms/         # TriangulationAlgorithms (Ear Clipping)
            ├── delone/
            │   └── algorithm/
            │       ├── DCEL.h, Dcel.cpp                # рёберный список
            │       ├── Delaunaytree.{h,cpp}            # локализация точки
            │       ├── Delaunaytriangulation.{h,cpp}   # инкрементальный алгоритм
            │       └── VoronoiFromDelaunay.{h,cpp}     # диаграмма Вороного
            └── boundingsets/
                └── algorithms/         # DiameterAlgorithms,
                                        # BoundingRectangle, BoundingCircle
```

Каждый модуль в `lib/src/geometry/` разделён на `core/` (структуры данных) и `algorithms/` (алгоритмы над ними).

---