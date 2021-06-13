QT += widgets
QT += opengl

CONFIG += c++11

SOURCES += \
        camera.cpp \
        graphicwindow.cpp \
        main.cpp \
        mainwidget.cpp \
        particles.cpp \
        worldmodels.cpp

HEADERS += \
    camera.h \
    graphicwindow.h \
    mainwidget.h \
    particles.h \
    worldmodels.h

RESOURCES += \
    shaders.qrc
