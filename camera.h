#pragma once

#include<math.h>

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Camera
{
public:
    Camera(const QVector3D &pos);

    void pitch(float degrees);
    void yaw(float degrees);
    void roll(float degrees);

    void walk(float amount);
    void strafe(float amount);
    void jump(float amount);

    void standXZ();
    void lookAtZero(float rate = 0.3f);

    QMatrix4x4 viewMatrix() const;

private:
    QVector3D m_pos;
    QVector3D m_forward;
    QVector3D m_right;
    QVector3D m_up;
    QQuaternion m_rot;
};
