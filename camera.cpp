#include "camera.h"

static inline void V3RotByQua(QVector3D& axis, const QQuaternion& rot)
{
    auto conjugateRot = rot.conjugated();
    auto quaAxis = QQuaternion(0.0f, axis);
    auto result = conjugateRot * quaAxis * rot;
    axis.setX(result.x());
    axis.setY(result.y());
    axis.setZ(result.z());
    //axis.normalize();
}

Camera::Camera(const QVector3D &pos)
    : m_pos(pos)
    , m_forward(0.0f, 0.0f, -1.0f)
    , m_right(1.0f, 0.0f, 0.0f)
    , m_up(0.0f, 1.0f, 0.0f)
{
}

QMatrix4x4 Camera::viewMatrix() const
{
    QMatrix4x4 matrix;
    matrix.rotate(m_rot);
    matrix.translate(m_pos);
    return matrix;
}

void Camera::pitch(float degrees)
{
    auto rot = QQuaternion::fromAxisAndAngle(m_right, degrees);
    m_rot *= rot;
    V3RotByQua(m_forward, rot);
    V3RotByQua(m_up, rot);
}

void Camera::yaw(float degrees)
{
    auto rot = QQuaternion::fromAxisAndAngle(m_up, degrees);
    m_rot *= rot;
    V3RotByQua(m_forward, rot);
    V3RotByQua(m_right, rot);
}

void Camera::roll(float degrees)
{
    auto rot = QQuaternion::fromAxisAndAngle(m_forward, degrees);
    m_rot *= rot;
    V3RotByQua(m_up, rot);
    V3RotByQua(m_right, rot);
}

void Camera::walk(float amount)
{
    m_pos += amount * m_forward;
}

void Camera::strafe(float amount)
{
    m_pos += amount * m_right;
}

void Camera::jump(float amount)
{
    m_pos += amount * m_up;
}

void Camera::standXZ()
{
    m_pos.setY(-0.3f);

    auto direction = QVector3D(0.0f, 1.0f, 0.0f);
    auto cosVal = QVector3D::dotProduct(direction, m_up);
    if (cosVal > 0.9999f)
        return;
    auto angle = acos(cosVal) * 0.3f;
    auto cross = QVector3D::crossProduct(direction, m_up);
    cross.normalize();
    auto rot = QQuaternion(cos(angle), sin(angle) * cross);
    rot.normalize();

    m_rot *= rot;
    V3RotByQua(m_forward, rot);
    V3RotByQua(m_right, rot);
    V3RotByQua(m_up, rot);

    m_rot.normalize();
    m_forward.normalize();
    m_right.normalize();
    m_up.normalize();
}

void Camera::lookAtZero(float rate)
{
    auto direction = m_pos.normalized();
    auto cosVal = QVector3D::dotProduct(direction, m_forward);
    if (cosVal > 0.9999f)
        return;
    auto angle = acos(cosVal) * rate;
    auto cross = QVector3D::crossProduct(direction, m_forward);
    cross.normalize();
    auto rot = QQuaternion(cos(angle), sin(angle) * cross);
    rot.normalize();

    m_rot *= rot;
    V3RotByQua(m_forward, rot);
    V3RotByQua(m_right, rot);
    V3RotByQua(m_up, rot);

    m_rot.normalize();
    m_forward.normalize();
    m_right.normalize();
    m_up.normalize();
}
