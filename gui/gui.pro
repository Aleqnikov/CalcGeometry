QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# 1. Пути к заголовочным файлам (чтобы работали #include без длинных путей)
INCLUDEPATH += . \
    ../lib/api \
    ../lib/src/linalgebra \
    ../lib/src/geometry/polygons/core \
    ../lib/src/geometry/polygons/algorithms

# 2. Все файлы реализации (именно их не хватало линковщику)
SOURCES += \
    controllers/BaseController.cpp \
    controllers/CircleController.cpp \
    controllers/DiameterController.cpp \
    controllers/HullController.cpp \
    controllers/PolygonController.cpp \
    controllers/RectangleController.cpp \
    controllers/TriangulationController.cpp \
    views/VisualizationWidget.cpp \
    dialogs/CircleSettingsDialog.cpp \
    dialogs/DiameterSettingsDialog.cpp \
    dialogs/HullSettingsDialogttingsdialog.cpp \
    dialogs/PolygonSettingsDialog.cpp \
    dialogs/TriangulationSettingsDialog.cpp \
    main.cpp \
    mainwindow.cpp \
    dialogs/RectangleSettingsDialog.cpp \
    dialogs/SettingsDialog.cpp \
    ../lib/api/GeometryApi.cpp \
    ../lib/src/linalgebra/LinAl.cpp \
    ../lib/src/geometry/polygons/core/Polygon.cpp \
    ../lib/src/geometry/polygons/core/ConvexPn.cpp \
    ../lib/src/geometry/polygons/core/StarPn.cpp \
    ../lib/src/geometry/polygons/algorithms/point_in_polygon.cpp \
    ../lib/src/geometry/triangulation/algorithms/TriangulationAlgorithms.cpp \
    ../lib/src/geometry/boundingsets/algorithms/BoundingCircle.cpp \
    ../lib/src/geometry/boundingsets/algorithms/BoundingRectangle.cpp \
    ../lib/src/geometry/boundingsets/algorithms/DiameterAlgorithms.cpp \
    ../lib/src/geometry/convexhull/core/Hull.cpp \
    ../lib/src/geometry/convexhull/algorithms/HullAlgorithms.cpp

# 3. Заголовки (для удобства отображения в дереве проекта QT Creator)
HEADERS += \
    controllers/BaseController.h \
    dialogs/CircleSettingsDialog.h \
    dialogs/DiameterSettingsDialog.h \
    dialogs/HullSettingsDialog.h \
    dialogs/PolygonSettingsDialog.h \
    dialogs/RectangleSettingsDialog.h \
    dialogs/SettingsDialog.h \
    controllers/CircleController.h \
    controllers/DiameterController.h \
    controllers/HullController.h \
    controllers/RectangleController.h \
    controllers/TriangulationController.h \
    dialogs/TriangulationSettingsDialog.h \
    jsonparcer.h \
    mainwindow.h \
    controllers/PolygonController.h \
    views/NavigationView.h \
    ../lib/api/GeometryApi.h \
    ../lib/src/linalgebra/Point2D.h \
    ../lib/src/linalgebra/LinAl.h \
    ../lib/src/geometry/polygons/core/Polygon.h \
    ../lib/src/geometry/polygons/core/ConvexPn.h \
    ../lib/src/geometry/polygons/core/StarPn.h \
    views/VisualizationWidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
