QT += widgets
QT += opengl

CONFIG += c++17

SOURCES += \
    camera.cpp \
    computeshaders.cpp \
    engine/abstractnbodyengine.cpp \
    engine/g3d/initializer3d.cpp \
    engine/g3d4d/initializer3d4d.cpp \
    engine/g3d4dr1/initializer3d4dr1.cpp \
    engine/g4d3d/initializer4d3d.cpp \
    graphicwindow.cpp \
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
    engine/abstractenginecore.h \
    engine/abstractenginecoredouble.h \
    engine/abstractenginecorefloat.h \
    engine/abstractinitializer.h \
    engine/abstractnbodyengine.h \
    engine/g3d/calculationg3d.h \
    engine/g3d/coredoubleg3d.h \
    engine/g3d/corefloatg3d.h \
    engine/g3d/engineg3d.h \
    engine/g3d/initializer3d.h \
    engine/g3d4d/calculationg3d4d.h \
    engine/g3d4d/coredoubleg3d4d.h \
    engine/g3d4d/corefloatg3d4d.h \
    engine/g3d4d/engineg3d4d.h \
    engine/g3d4d/initializer3d4d.h \
    engine/g3d4dr1/calculationg3d4dr1.h \
    engine/g3d4dr1/coredoubleg3d4dr1.h \
    engine/g3d4dr1/corefloatg3d4dr1.h \
    engine/g3d4dr1/engineg3d4dr1.h \
    engine/g3d4dr1/initializer3d4dr1.h \
    engine/g4d3d/calculationg4d3d.h \
    engine/g4d3d/coredoubleg4d3d.h \
    engine/g4d3d/corefloatg4d3d.h \
    engine/g4d3d/engineg4d3d.h \
    engine/g4d3d/initializer4d3d.h \
    global.h \
    graphicwindow.h \
    hnn/constants.h \
    hnn/octonion.h \
    hnn/quaternion.h \
    hnn/vector3.h \
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
