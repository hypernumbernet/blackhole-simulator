QT += widgets
QT += opengl

CONFIG += c++17

SOURCES += \
    GraphDialog.cpp \
    ThreadAdmin.cpp \
    ThreadController.cpp \
    UpdateUi.cpp \
    bhs.cpp \
    Camera.cpp \
    ComputeShaders.cpp \
    engine/3D/Initializer3D.cpp \
    engine/AbstractNBodyEngine.cpp \
    engine/GravityCollision/InitializerGravityCollision.cpp \
    engine/Relativity1/InitializerRelativity1.cpp \
    engine/Relativity2/InitializerRelativity2.cpp \
    engine/Relativity3/InitializerRelativity3.cpp \
    GraphicWindow.cpp \
    InitializerDialog.cpp \
    LineShaders.cpp \
    engine/Universe1/InitializerUniverse1.cpp \
    engine/Universe2/InitializerUniverse2.cpp \
    main.cpp \
    MainWidget.cpp \
    MonitoringDialog.cpp \
    ParticleShaders.cpp

HEADERS += \
    GraphDialog.h \
    ThreadAdmin.h \
    ThreadController.h \
    UpdateUi.h \
    bhs.h \
    Camera.h \
    ComputeShaders.h \
    engine/3D/Calculation3D.h \
    engine/3D/CoreDouble3D.h \
    engine/3D/Engine3D.h \
    engine/3D/Initializer3D.h \
    engine/AbstractEngineCore.h \
    engine/AbstractEngineCoreDouble.h \
    engine/AbstractInitializer.h \
    engine/AbstractNBodyEngine.h \
    engine/CoreEvenlyDivided.h \
    engine/CoreTrapezoid.h \
    engine/GravityCollision/CalculationGravityCollision.h \
    engine/GravityCollision/CoreDoubleGravityCollision.h \
    engine/GravityCollision/EngineGravityCollision.h \
    engine/GravityCollision/InitializerGravityCollision.h \
    engine/Relativity1/CalculationRelativity1.h \
    engine/Relativity1/CoreDoubleRelativity1.h \
    engine/Relativity1/EngineRelativity1.h \
    engine/Relativity1/InitializerRelativity1.h \
    engine/Relativity2/CalculationRelativity2.h \
    engine/Relativity2/CoreDoubleRelativity2.h \
    engine/Relativity2/EngineRelativity2.h \
    engine/Relativity2/InitializerRelativity2.h \
    engine/Relativity3/CalculationRelativity3.h \
    engine/Relativity3/CoreDoubleRelativity3.h \
    engine/Relativity3/EngineRelativity3.h \
    engine/Relativity3/InitializerRelativity3.h \
    GraphicWindow.h \
    engine/Universe1/CalculationUniverse1.h \
    engine/Universe1/CoreDoubleUniverse1.h \
    engine/Universe1/EngineUniverse1.h \
    engine/Universe1/InitializerUniverse1.h \
    engine/Universe2/CalculationUniverse2.h \
    engine/Universe2/CoreDoubleUniverse2.h \
    engine/Universe2/EngineUniverse2.h \
    engine/Universe2/InitializerUniverse2.h \
    hnn/biquaternion.h \
    hnn/constants.h \
    hnn/octonion.h \
    hnn/quaternion.h \
    hnn/spacetime.h \
    hnn/splitoctonion.h \
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
