QT += widgets
QT += opengl

CONFIG += c++17

SOURCES += \
    UpdateUi.cpp \
    camera.cpp \
    computeshaders.cpp \
    engine/GravityCollision/InitializerGravityCollision.cpp \
    engine/abstractnbodyengine.cpp \
    engine/3d/initializer3d.cpp \
    engine/3d4d/initializer3d4d.cpp \
    engine/3d4dr1/initializer3d4dr1.cpp \
    engine/4d3d/initializer4d3d.cpp \
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
    engine/GravityCollision/CalculationGravityCollision.h \
    engine/GravityCollision/CoreDoubleGravityCollision.h \
    engine/GravityCollision/CoreFloatGravityCollision.h \
    engine/GravityCollision/EngineGravityCollision.h \
    engine/GravityCollision/InitializerGravityCollision.h \
    engine/abstractenginecore.h \
    engine/abstractenginecoredouble.h \
    engine/abstractenginecorefloat.h \
    engine/abstractinitializer.h \
    engine/abstractnbodyengine.h \
    engine/3d/calculation3d.h \
    engine/3d/coredouble3d.h \
    engine/3d/corefloat3d.h \
    engine/3d/engine3d.h \
    engine/3d/initializer3d.h \
    engine/3d4d/calculation3d4d.h \
    engine/3d4d/coredouble3d4d.h \
    engine/3d4d/corefloat3d4d.h \
    engine/3d4d/engine3d4d.h \
    engine/3d4d/initializer3d4d.h \
    engine/3d4dr1/calculation3d4dr1.h \
    engine/3d4dr1/coredouble3d4dr1.h \
    engine/3d4dr1/corefloat3d4dr1.h \
    engine/3d4dr1/engine3d4dr1.h \
    engine/3d4dr1/initializer3d4dr1.h \
    engine/4d3d/calculation4d3d.h \
    engine/4d3d/coredouble4d3d.h \
    engine/4d3d/corefloat4d3d.h \
    engine/4d3d/engine4d3d.h \
    engine/4d3d/initializer4d3d.h \
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

if(win32-msvc*):lessThan(QT_VERSION, 6){
    QMAKE_CXXFLAGS += /source-charset:utf-8
}
