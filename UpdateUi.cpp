#include "UpdateUi.h"

UpdateUi& UpdateUi::it()
{
    static UpdateUi instance;
    return instance;
}

QHash<bhs::Engine, QString>& UpdateUi::engineLabel()
{
    static QHash<bhs::Engine, QString> map = {
        {bhs::Engine::Gravity3D, tr("Gravity")},
        {bhs::Engine::GravityCollision, tr("Gravity and Collision")},
        {bhs::Engine::Relativity1, tr("Relativity 1")},
        {bhs::Engine::Relativity2, tr("Relativity 2")},
        {bhs::Engine::Relativity3, tr("Relativity 3")},
        {bhs::Engine::Universe1, tr("Universe1")},
        {bhs::Engine::Universe2, tr("Universe2")},
    };
    return map;
}

QList<bhs::Engine>& UpdateUi::engineUse()
{
    static QList<bhs::Engine> list = {
        bhs::Engine::Gravity3D,
        bhs::Engine::GravityCollision,
        bhs::Engine::Relativity1,
        bhs::Engine::Relativity2,
        bhs::Engine::Relativity3,
        bhs::Engine::Universe1,
        bhs::Engine::Universe2,
    };
    return list;
}

QMap<bhs::Precision, QString>& UpdateUi::precision()
{
    static QMap<bhs::Precision, QString> map = {
        {bhs::Precision::Float, tr("Float")},
        {bhs::Precision::Double, tr("Double")},
    };
    return map;
}

QMap<bhs::Compute, QString>& UpdateUi::compute()
{
    static QMap<bhs::Compute, QString> map = {
        {bhs::Compute::CPU, tr("CPU")},
        {bhs::Compute::GPU, tr("GPU")},
    };
    return map;
}

QMap<bhs::Preset, QString>& UpdateUi::preset()
{
    static QMap<bhs::Preset, QString> map = {
        {bhs::Preset::RandomCube, tr("Random Cube")},
        {bhs::Preset::RandomSphere, tr("Random Sphere")},
        {bhs::Preset::RandomBall, tr("Random Ball")},
        {bhs::Preset::Custom, tr("Custom")},
    };
    return map;
}

QMap<bhs::LineType, QString>& UpdateUi::lineType()
{
    static QMap<bhs::LineType, QString> map = {
        {bhs::LineType::XZMeshes, tr("XZ Meshes")},
        {bhs::LineType::Axis, tr("Basic Axis")},
        {bhs::LineType::CubeMeshes, tr("Cube")},
        {bhs::LineType::LongitudeAndLatitude, tr("Longitude Latitude")},
        {bhs::LineType::QuaternionS3Rotation, tr("Quaternion S3 Rotation")},
        {bhs::LineType::OctonionS3RotationXY, tr("Octonion S3 Rotation XY")},
        {bhs::LineType::OctonionS3RotationAll, tr("Octonion S3 Rotation All")},
    };
    return map;
}

void UpdateUi::style(QLCDNumber& lcd)
{
    lcd.setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    lcd.setSegmentStyle(QLCDNumber::Flat);
}

void UpdateUi::style(QLabel& lbl)
{
    lbl.setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    lbl.setAlignment(Qt::AlignRight);
    lbl.setMargin(2);
}

void UpdateUi::width(QLabel& lbl)
{
    lbl.setMaximumWidth(50);
}

QString UpdateUi::debugString(quintptr ptr)
{
    return QString("0x%1").arg(ptr, QT_POINTER_SIZE * 2, 16, QChar('0'));
}
