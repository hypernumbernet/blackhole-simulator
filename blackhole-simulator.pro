QT += widgets
QT += opengl

CONFIG += c++17

SOURCES += \
    abstractinitializer.cpp \
    abstractnbodyengine.cpp \
    calculatetimeprogress.cpp \
    camera.cpp \
    graphicwindow.cpp \
    initializer3d.cpp \
    initializerdialog.cpp \
    main.cpp \
    mainwidget.cpp \
    particles.cpp \
    threadadmin.cpp \
    threadcontroller.cpp \
    worldmodels.cpp

HEADERS += \
    abstractinitializer.h \
    abstractnbodyengine.h \
    calculatetimeprogress.h \
    camera.h \
    g3dmassdiffnbe.h \
    g3dmassintegralnbe.h \
    g3svmassdiffnbe.h \
    graphicwindow.h \
    initializer3d.h \
    initializerdialog.h \
    mainwidget.h \
    particles.h \
    quaternion.h \
    threadadmin.h \
    threadcontroller.h \
    updateui.h \
    vector3.h \
    worldmodels.h

RESOURCES += \
    shaders.qrc

win32-msvc*{
    QMAKE_CXXFLAGS += /source-charset:utf-8
}
