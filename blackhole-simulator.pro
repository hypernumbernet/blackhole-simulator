QT += widgets
QT += opengl

CONFIG += c++17

SOURCES += \
    abstractnbodyengine.cpp \
    camera.cpp \
    graphicwindow.cpp \
    initializer3d.cpp \
    initializer3d4d.cpp \
    initializerdialog.cpp \
    main.cpp \
    mainwidget.cpp \
    particles.cpp \
    threadadmin.cpp \
    threadcontroller.cpp \
    worldmodels.cpp

HEADERS += \
    abstractenginecore.h \
    abstractenginecoredouble.h \
    abstractenginecoresingle.h \
    abstractinitializer.h \
    abstractnbodyengine.h \
    camera.h \
    g3d4dmassdiffcoredouble.h \
    g3d4dmassdiffcoresingle.h \
    g3d4dmassdiffnbe.h \
    g3dmassdiffcoredouble.h \
    g3dmassdiffcoresingle.h \
    g3dmassdiffnbe.h \
    g3dmassintegralcoredouble.h \
    g3dmassintegralcoresingle.h \
    g3dmassintegralnbe.h \
    graphicwindow.h \
    initializer3d.h \
    initializer3d4d.h \
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
