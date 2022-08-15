#pragma once

#include <cmath>

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QMutex>

#include "hnn/constants.h"

class Camera
{
public:
    explicit Camera(const QVector3D& pos);

    static void rotateV3ByQuaternion(QVector3D& axis, const QQuaternion& rot);

    void pitch(float degrees);
    void yaw(float degrees);
    void roll(float degrees);

    void walk(float amount);
    void strafe(float amount);
    void jump(float amount);

    bool standXZ(bool resetY = true, float rate = 0.6f);
    void topY(float rate = 0.6f);
    void lookAtZero(float rate = 0.6f);
    void lookAt(const QVector3D& point, float rate = 0.6f);
    bool setPosition(const QVector3D&, float rate = 0.6f);
    void circleStrafing(float amount);
    void roundUp(float amount);
    void reset(const QVector3D& position);

    QMatrix4x4 viewMatrix() const;

private:
    void multiplyRotation(const QQuaternion&);
    QQuaternion slerp(const QVector3D&, const QVector3D&, float rate = 1.0f, float threshold = 1.0f);

    QVector3D m_position;
    QVector3D m_forward;
    QVector3D m_right;
    QVector3D m_up;
    QQuaternion m_rotation;
    QMutex m_mutex;
};
