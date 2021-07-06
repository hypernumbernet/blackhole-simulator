QT += widgets
QT += opengl

CONFIG += c++17

SOURCES += \
    abstractnbodyengine.cpp \
    camera.cpp \
    computeshaders.cpp \
    graphicwindow.cpp \
    initializer3d.cpp \
    initializer3d4d.cpp \
    initializerdialog.cpp \
    lineshaders.cpp \
    main.cpp \
    mainwidget.cpp \
    particleshaders.cpp \
    threadadmin.cpp \
    threadcontroller.cpp

HEADERS += \
    abstractenginecore.h \
    abstractenginecoredouble.h \
    abstractenginecorefloat.h \
    abstractinitializer.h \
    abstractnbodyengine.h \
    camera.h \
    computeshaders.h \
    g3d4deulercoredouble.h \
    g3d4deulercorefloat.h \
    g3d4deulerengine.h \
    g3deulercoredouble.h \
    g3deulercorefloat.h \
    g3deulerengine.h \
    global.h \
    graphicwindow.h \
    initializer3d.h \
    initializer3d4d.h \
    initializerdialog.h \
    lineshaders.h \
    mainwidget.h \
    particleshaders.h \
    quaternion.h \
    threadadmin.h \
    threadcontroller.h \
    updateui.h \
    vector3.h

RESOURCES += \
    shaders.qrc

win32-msvc*{
    QMAKE_CXXFLAGS += /source-charset:utf-8
}

DISTFILES +=
