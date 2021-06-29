QT += widgets
QT += opengl

CONFIG += c++17

SOURCES += \
    abstractenginecoredouble.cpp \
    abstractenginecoresingle.cpp \
    abstractnbodyengine.cpp \
    camera.cpp \
    g3dmassdiffcore.cpp \
    g3dmassdiffcoredouble.cpp \
    g3dmassintegralcore.cpp \
    g3svmassdiffcore.cpp \
    graphicwindow.cpp \
    initializer3d.cpp \
    initializer3d3s.cpp \
    initializerdialog.cpp \
    main.cpp \
    mainwidget.cpp \
    particles.cpp \
    threadadmin.cpp \
    threadcontroller.cpp \
    worldmodels.cpp

HEADERS += \
    abstractenginecoredouble.h \
    abstractenginecoresingle.h \
    abstractinitializer.h \
    abstractnbodyengine.h \
    camera.h \
    g3dmassdiffcore.h \
    g3dmassdiffcoredouble.h \
    g3dmassdiffnbe.h \
    g3dmassintegralcore.h \
    g3dmassintegralnbe.h \
    g3svmassdiffcore.h \
    g3svmassdiffnbe.h \
    graphicwindow.h \
    initializer3d.h \
    initializer3d3s.h \
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
