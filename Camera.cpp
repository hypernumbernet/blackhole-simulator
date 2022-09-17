#include "Camera.h"

Camera::Camera(const Vector3& pos)
    : m_position(pos)
    , m_forward(0.0, 0.0, -1.0)
    , m_right(1.0, 0.0, 0.0)
    , m_up(0.0, 1.0, 0.0)
    , m_rotation(1.0)
{
}

QMatrix4x4 Camera::viewMatrix() const
{
    QMatrix4x4 matrix;
    matrix.rotate(QQuaternion((float)m_rotation.re(), (float)m_rotation.i1(),
                              (float)m_rotation.i2(), (float)m_rotation.i3()));
    matrix.translate(QVector3D((float)m_position.x(), (float)m_position.y(), (float)m_position.z()));
    return matrix;
}

void Camera::pitch(const double degrees)
{
    QMutexLocker locker(&m_mutex);
    auto rot = Quaternion::rotation(m_right, degrees);
    multiplyRotation(rot);
}

void Camera::yaw(const double degrees)
{
    QMutexLocker locker(&m_mutex);
    auto rot = Quaternion::rotation(m_up, degrees);
    multiplyRotation(rot);
}

void Camera::roll(const double degrees)
{
    QMutexLocker locker(&m_mutex);
    auto rot = Quaternion::rotation(m_forward, degrees);
    multiplyRotation(rot);
}

void Camera::walk(const double amount)
{
    m_position += amount * m_forward;
}

void Camera::strafe(const double amount)
{
    m_position += amount * m_right;
}

void Camera::jump(const double amount)
{
    m_position += amount * m_up;
}

bool Camera::standXZ(const bool resetY, const double rate)
{
    QMutexLocker locker(&m_mutex);
    if (resetY)
        m_position.setY(m_position.y() + rate * (-1.0 - m_position.y()));

    auto direction = Vector3(0.0, 1.0, 0.0);
    auto rot = Quaternion::slerp(direction, m_up, rate, 0.999);
    if (rot.re() == 1.0)
        return false;

    multiplyRotation(rot);

    return true;
}

void Camera::topY(const double rate)
{
    QMutexLocker locker(&m_mutex);
    auto position = m_position.normalized();

    auto rot = Quaternion::slerp(position, m_forward, rate, 0.99999);
    if (rot.re() < 1.0)
        multiplyRotation(rot);

    auto direction = Vector3(0.0, 1.0, 0.0);
    auto cosY = position.dot(direction);
    if (cosY < 0.99 && cosY > -0.99)
    {
        auto angleY = (acos(cosY) - hnn::PI * 0.5) * 0.5;
        auto crossY = position.cross(direction);
        crossY.normalize();
        auto rotY = Quaternion::rotation(crossY, angleY);
        Quaternion::rotate(direction, rotY);
        auto rotU = Quaternion::slerp(direction, m_up, rate);
        if (rotU.re() < 1.0)
            multiplyRotation(rotU);
    }
}

void Camera::lookAtZero(const double rate)
{
    lookAt({0.0, 0.0, 0.0}, rate);
}

void Camera::lookAt(const Vector3& point, const double rate)
{
    QMutexLocker locker(&m_mutex);
    auto direction = (m_position - point).normalized();
    auto rot = Quaternion::slerp(direction, m_forward, rate);
    multiplyRotation(rot);
}

bool Camera::setPosition(const Vector3& pos, const double rate)
{
    QMutexLocker locker(&m_mutex);
    if (rate >= 1.0)
    {
        m_position = pos;
        return true;
    }
    double norm = m_position.distance(pos);
    if (norm < 0.01)
        return false;
    Vector3 r = (pos - m_position).normalized();
    m_position += rate * norm * r;
    return true;
}

void Camera::circleStrafing(const double amount)
{
    m_position += amount * m_right;
    lookAtZero(1.0);
}

void Camera::roundUp(const double amount)
{
    m_position += amount * m_up;
    lookAtZero(1.0);
}

void Camera::reset(const Vector3& position)
{
    QMutexLocker locker(&m_mutex);
    m_position = position;
    m_rotation = Quaternion(1.0);
    m_forward = Vector3(0.0, 0.0, -1.0);
    m_right = Vector3(1.0, 0.0, 0.0);
    m_up = Vector3(0.0, 1.0, 0.0);
}

void Camera::multiplyRotation(const Quaternion& rot)
{
    m_rotation *= rot;
    m_rotation.normalize();
    Quaternion::rotate(m_forward, rot);
    Quaternion::rotate(m_right, rot);
    Quaternion::rotate(m_up, rot);
}
