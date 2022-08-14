QT += widgets
QT += opengl

CONFIG += c++17

SOURCES += \
    camera.cpp \
    computeshaders.cpp \
    engine/abstractnbodyengine.cpp \
    graphicwindow.cpp \
    initializer/initializer3d.cpp \
    initializer/initializer3d4d.cpp \
    initializer/initializer3d4dr1.cpp \
    initializer/initializer4d3d.cpp \
    initializerdialog.cpp \
    lineshaders.cpp \
    main.cpp \
    mainwidget.cpp \
    monitoringdialog.cpp \
    particleshaders.cpp \
    threadadmin.cpp \
    threadcontroller.cpp

HEADERS += \
    camera.h \
    computeshaders.h \
    engine/abstractnbodyengine.h \
    engine/abstractenginecore.h \
    engine/abstractenginecoredouble.h \
    engine/abstractenginecorefloat.h \
    engine/g3d/g3dcalculation.h \
    engine/g3d/g3dcoredouble.h \
    engine/g3d/g3dcorefloat.h \
    engine/g3d/g3dengine.h \
    engine/g3d4d/g3d4dcalculation.h \
    engine/g3d4d/g3d4dcoredouble.h \
    engine/g3d4d/g3d4dcorefloat.h \
    engine/g3d4d/g3d4dengine.h \
    engine/g3d4dr1/g3d4dr1calculation.h \
    engine/g3d4dr1/g3d4dr1coredouble.h \
    engine/g3d4dr1/g3d4dr1corefloat.h \
    engine/g3d4dr1/g3d4dr1engine.h \
    engine/g4d3d/g4d3dcalculation.h \
    engine/g4d3d/g4d3dcoredouble.h \
    engine/g4d3d/g4d3dcorefloat.h \
    engine/g4d3d/g4d3dengine.h \
    global.h \
    graphicwindow.h \
    hnn/constants.h \
    hnn/octonion.h \
    hnn/quaternion.h \
    hnn/vector3.h \
    initializer/abstractinitializer.h \
    initializer/initializer3d.h \
    initializer/initializer3d4d.h \
    initializer/initializer3d4dr1.h \
    initializer/initializer4d3d.h \
    initializerdialog.h \
    lineshaders.h \
    mainwidget.h \
    monitoringdialog.h \
    particleshaders.h \
    threadadmin.h \
    threadcontroller.h \
    updateui.h \

RESOURCES += \
    shaders.qrc

win32-msvc*{
    QMAKE_CXXFLAGS += /source-charset:utf-8
}
