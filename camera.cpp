#include "camera.h"

static inline void V3RotByQua(QVector3D& axis, const QQuaternion& rot)
{
    auto conjugateRot = rot.conjugated();
    auto quaAxis = QQuaternion(0.0f, axis);
    auto result = conjugateRot * quaAxis * rot;
    axis.setX(result.x());
    axis.setY(result.y());
    axis.setZ(result.z());

    // 計算毎に正規化したいなら
    //axis.normalize();

    // 以下は動かない。角度が倍？
    //axis = rot.rotatedVector(axis);
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

void Camera::pitch(const float degrees)
{
    auto rot = QQuaternion::fromAxisAndAngle(m_right, degrees);
    m_rot *= rot;
    V3RotByQua(m_forward, rot);
    V3RotByQua(m_up, rot);
}

void Camera::yaw(const float degrees)
{
    auto rot = QQuaternion::fromAxisAndAngle(m_up, degrees);
    m_rot *= rot;
    V3RotByQua(m_forward, rot);
    V3RotByQua(m_right, rot);
}

void Camera::roll(const float degrees)
{
    auto rot = QQuaternion::fromAxisAndAngle(m_forward, degrees);
    m_rot *= rot;
    V3RotByQua(m_up, rot);
    V3RotByQua(m_right, rot);
}

void Camera::walk(const float amount)
{
    m_pos += amount * m_forward;
}

void Camera::strafe(const float amount)
{
    m_pos += amount * m_right;
}

void Camera::jump(const float amount)
{
    m_pos += amount * m_up;
}

bool Camera::standXZ(const bool resetY, const float rate)
{
    if (resetY)
        m_pos.setY(-1.0f);

    auto direction = QVector3D(0.0f, 1.0f, 0.0f);
    auto cosVal = QVector3D::dotProduct(direction, m_up);
    if (cosVal > 0.999f)
        return false;
    auto angle = acos(cosVal) * rate * 0.5f;
    auto cross = QVector3D::crossProduct(direction, m_up);
    cross.normalize();
    auto rot = QQuaternion(cos(angle), sin(angle) * cross);
    rot.normalize();

    m_rot *= rot;
    V3RotByQua(m_forward, rot);
    V3RotByQua(m_right, rot);
    V3RotByQua(m_up, rot);

    // 念のための正規化。実際はあまりズレない？
    m_rot.normalize();
    m_forward.normalize();
    m_right.normalize();
    m_up.normalize();
    return true;
}

bool Camera::lookAtZero(const float rate)
{
    auto direction = m_pos.normalized();
    auto cosVal = QVector3D::dotProduct(direction, m_forward);
    if (cosVal > 0.999f)
        return false;
    auto angle = acos(cosVal) * rate * 0.5f;
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
    return true;
}

bool Camera::lookAt(const QVector3D point, const float rate)
{
    //auto direction = (point - m_pos).normalized();
    auto direction = (m_pos - point).normalized();
    auto cosVal = QVector3D::dotProduct(direction, m_forward);
    if (cosVal > 0.999f)
        return false;
    auto angle = acos(cosVal) * rate * 0.5f;
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
    return true;
}

bool Camera::setPosition(const QVector3D pos, const float rate)
{
    float norm = m_pos.distanceToPoint(pos);
    if (norm < 0.01f)
        return false;
    QVector3D r = (pos - m_pos).normalized();
    m_pos += rate * norm * r;
    return true;
}
