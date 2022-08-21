#pragma once

#include "hnn/quaternion.h"
#include <QMatrix4x4>
#include <QMutex>

using namespace hnn;

class Camera
{
public:
    explicit Camera(const Vector3& pos);

    void pitch(double degrees);
    void yaw(double degrees);
    void roll(double degrees);

    void walk(double amount);
    void strafe(double amount);
    void jump(double amount);

    bool standXZ(bool resetY = true, double rate = 0.6);
    void topY(double rate = 0.6);
    void lookAtZero(double rate = 0.6);
    void lookAt(const Vector3& point, double rate = 0.6);
    bool setPosition(const Vector3&, double rate = 0.6);
    void circleStrafing(double amount);
    void roundUp(double amount);
    void reset(const Vector3& position);

    QMatrix4x4 viewMatrix() const;

private:
    void multiplyRotation(const Quaternion&);
    Vector3 m_position;
    Vector3 m_forward;
    Vector3 m_right;
    Vector3 m_up;
    Quaternion m_rotation;
    QMutex m_mutex;
};
