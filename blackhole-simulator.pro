QT += widgets
QT += opengl

CONFIG += c++17

SOURCES += \
    ThreadAdmin.cpp \
    ThreadController.cpp \
    UpdateUi.cpp \
    bhs.cpp \
    Camera.cpp \
    ComputeShaders.cpp \
    engine/GravityCollision/InitializerGravityCollision.cpp \
    engine/Relativity2/InitializerRelativity2.cpp \
    engine/Relativity3/InitializerRelativity3.cpp \
    engine/abstractnbodyengine.cpp \
    engine/3d/initializer3d.cpp \
    engine/3d4d/initializer3d4d.cpp \
    engine/3d4dr1/initializer3d4dr1.cpp \
    engine/4d3d/initializer4d3d.cpp \
    GraphicWindow.cpp \
    InitializerDialog.cpp \
    LineShaders.cpp \
    main.cpp \
    MainWidget.cpp \
    MonitoringDialog.cpp \
    ParticleShaders.cpp

HEADERS += \
    ThreadAdmin.h \
    ThreadController.h \
    UpdateUi.h \
    bhs.h \
    Camera.h \
    ComputeShaders.h \
    engine/CoreEvenlyDivided.h \
    engine/CoreTrapezoid.h \
    engine/GravityCollision/CalculationGravityCollision.h \
    engine/GravityCollision/CoreDoubleGravityCollision.h \
    engine/GravityCollision/EngineGravityCollision.h \
    engine/GravityCollision/InitializerGravityCollision.h \
    engine/Relativity2/CalculationRelativity2.h \
    engine/Relativity2/CoreDoubleRelativity2.h \
    engine/Relativity2/EngineRelativity2.h \
    engine/Relativity2/InitializerRelativity2.h \
    engine/Relativity3/CalculationRelativity3.h \
    engine/Relativity3/CoreDoubleRelativity3.h \
    engine/Relativity3/EngineRelativity3.h \
    engine/Relativity3/InitializerRelativity3.h \
    engine/abstractenginecore.h \
    engine/abstractenginecoredouble.h \
    engine/abstractinitializer.h \
    engine/abstractnbodyengine.h \
    engine/3d/calculation3d.h \
    engine/3d/coredouble3d.h \
    engine/3d/engine3d.h \
    engine/3d/initializer3d.h \
    engine/3d4d/calculation3d4d.h \
    engine/3d4d/coredouble3d4d.h \
    engine/3d4d/engine3d4d.h \
    engine/3d4d/initializer3d4d.h \
    engine/3d4dr1/calculation3d4dr1.h \
    engine/3d4dr1/coredouble3d4dr1.h \
    engine/3d4dr1/engine3d4dr1.h \
    engine/3d4dr1/initializer3d4dr1.h \
    engine/4d3d/calculation4d3d.h \
    engine/4d3d/coredouble4d3d.h \
    engine/4d3d/engine4d3d.h \
    engine/4d3d/initializer4d3d.h \
    GraphicWindow.h \
    hnn/biquaternion.h \
    hnn/constants.h \
    hnn/octonion.h \
    hnn/quaternion.h \
    hnn/spacetime.h \
    hnn/vector3.h \
    InitializerDialog.h \
    LineShaders.h \
    MainWidget.h \
    MonitoringDialog.h \
    ParticleShaders.h \

RESOURCES += \
    shaders.qrc

if(win32-msvc*):lessThan(QT_VERSION, 6){
    QMAKE_CXXFLAGS += /source-charset:utf-8
}
