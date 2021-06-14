QT += widgets
QT += opengl

CONFIG += c++17

SOURCES += \
        abstractnbodyengine.cpp \
        camera.cpp \
        graphicwindow.cpp \
        gravity3dmassdifferentialnbodyengine.cpp \
        main.cpp \
        mainwidget.cpp \
        particles.cpp \
        worldmodels.cpp

HEADERS += \
    abstractnbodyengine.h \
    camera.h \
    graphicwindow.h \
    gravity3dmassdifferentialnbodyengine.h \
    mainwidget.h \
    particles.h \
    worldmodels.h

RESOURCES += \
    shaders.qrc

win32-msvc*{
    QMAKE_CXXFLAGS += /source-charset:utf-8
}
