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
    main.cpp \
    mainwindow.cpp \
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
    CircleController.h \
    DiameterController.h \
    HullController.h \
    RectangleController.h \
    TriangulationController.h \
    jsonparcer.h \
    mainwindow.h \
    PolygonController.h \
    NavigationView.h \
    ../lib/api/GeometryApi.h \
    ../lib/src/linalgebra/Point2D.h \
    ../lib/src/linalgebra/LinAl.h \
    ../lib/src/geometry/polygons/core/Polygon.h \
    ../lib/src/geometry/polygons/core/ConvexPn.h \
    ../lib/src/geometry/polygons/core/StarPn.h

FORMS += \
    mainwindow.ui

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
