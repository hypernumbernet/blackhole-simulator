QT += widgets
QT += opengl

CONFIG += c++17

SOURCES += \
    abstractinitializer.cpp \
        abstractnbodyengine.cpp \
        camera.cpp \
    g3dmassdiffnbe.cpp \
    g3dmassintegralnbe.cpp \
        graphicwindow.cpp \
    initializer3d.cpp \
    initializerdialog.cpp \
        main.cpp \
        mainwidget.cpp \
        particles.cpp \
#        updateui.h \
        worldmodels.cpp

HEADERS += \
    abstractinitializer.h \
    abstractnbodyengine.h \
    camera.h \
    g3dmassdiffnbe.h \
    g3dmassintegralnbe.h \
    graphicwindow.h \
    initializer3d.h \
    initializerdialog.h \
    mainwidget.h \
    particles.h \
    updateui.h \
    worldmodels.h

RESOURCES += \
    shaders.qrc

win32-msvc*{
    QMAKE_CXXFLAGS += /source-charset:utf-8
}
