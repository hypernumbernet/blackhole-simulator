QT += widgets
QT += opengl

CONFIG += c++17

SOURCES += \
    G3DMassIntegralNBodyEngine.cpp \
        abstractnbodyengine.cpp \
        camera.cpp \
        graphicwindow.cpp \
        gravity3dmassdifferentialnbodyengine.cpp \
    initializerdialog.cpp \
        main.cpp \
        mainwidget.cpp \
        particles.cpp \
#        updateui.h \
        worldmodels.cpp

HEADERS += \
    G3DMassIntegralNBodyEngine.h \
    abstractnbodyengine.h \
    camera.h \
    graphicwindow.h \
    gravity3dmassdifferentialnbodyengine.h \
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
