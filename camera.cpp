#include "camera.h"

void Camera::rotateV3ByQuaternion(QVector3D& axis, const QQuaternion& rot)
{
    // The following calculation did not work.
    //axis = rot.rotatedVector(axis);

    auto conjugateRot = rot.conjugated();
    QQuaternion quaAxis(0.0f, axis);
    auto result = conjugateRot * quaAxis * rot;
    axis.setX(result.x());
    axis.setY(result.y());
    axis.setZ(result.z());

    axis.normalize();
}

QQuaternion Camera::slerp(const QVector3D& from, const QVector3D& to, float rate, float threshold)
{
    // QQuaternion::slerp did not work well.

    auto cosVal = QVector3D::dotProduct(from, to);
    if (cosVal >= threshold || cosVal <= -threshold)
        return QQuaternion(1.0f, 0.0f, 0.0f, 0.0f);
    auto angle = acos(cosVal) * rate * 0.5f;
    auto cross = QVector3D::crossProduct(from, to);
    cross.normalize();
    auto rot = QQuaternion(cos(angle), sin(angle) * cross);
    rot.normalize();
    return rot;
}

Camera::Camera(const QVector3D &pos)
    : m_position(pos)
    , m_forward(0.0f, 0.0f, -1.0f)
    , m_right(1.0f, 0.0f, 0.0f)
    , m_up(0.0f, 1.0f, 0.0f)
{
}

QMatrix4x4 Camera::viewMatrix() const
{
    QMatrix4x4 matrix;
    matrix.rotate(m_rotation);
    matrix.translate(m_position);
    return matrix;
}

void Camera::pitch(const float degrees)
{
    QMutexLocker locker(&m_mutex);
    auto rot = QQuaternion::fromAxisAndAngle(m_right, degrees);
    multiplyRotation(rot);
    rotateV3ByQuaternion(m_forward, rot);
    rotateV3ByQuaternion(m_up, rot);
}

void Camera::yaw(const float degrees)
{
    QMutexLocker locker(&m_mutex);
    auto rot = QQuaternion::fromAxisAndAngle(m_up, degrees);
    multiplyRotation(rot);
    rotateV3ByQuaternion(m_forward, rot);
    rotateV3ByQuaternion(m_right, rot);
}

void Camera::roll(const float degrees)
{
    QMutexLocker locker(&m_mutex);
    auto rot = QQuaternion::fromAxisAndAngle(m_forward, degrees);
    multiplyRotation(rot);
    rotateV3ByQuaternion(m_up, rot);
    rotateV3ByQuaternion(m_right, rot);
}

void Camera::walk(const float amount)
{
    m_position += amount * m_forward;
}

void Camera::strafe(const float amount)
{
    m_position += amount * m_right;
}

void Camera::jump(const float amount)
{
    m_position += amount * m_up;
}

bool Camera::standXZ(const bool resetY, const float rate)
{
    QMutexLocker locker(&m_mutex);
    if (resetY)
        m_position.setY(m_position.y() + rate * (-1.0f - m_position.y()));

    auto direction = QVector3D(0.0f, 1.0f, 0.0f);
    auto rot = slerp(direction, m_up, rate, 0.999f);
    if (rot.scalar() == 1.0f)
        return false;

    multiplyRotation(rot);
    rotateV3ByQuaternion(m_forward, rot);
    rotateV3ByQuaternion(m_right, rot);
    rotateV3ByQuaternion(m_up, rot);

    return true;
}

void Camera::topY(const float rate)
{
    QMutexLocker locker(&m_mutex);
    auto position = m_position.normalized();

    auto rot = slerp(position, m_forward, rate, 0.99999f);
    if (rot.scalar() < 1.0f)
    {
        multiplyRotation(rot);
        rotateV3ByQuaternion(m_forward, rot);
        rotateV3ByQuaternion(m_right, rot);
        rotateV3ByQuaternion(m_up, rot);
    }

    auto direction = QVector3D(0.0f, 1.0f, 0.0f);
    auto cosY = QVector3D::dotProduct(position, direction);
    if (cosY < 0.99f && cosY > -0.99f)
    {
        auto angleY = (acos(cosY) - float(hnn::PI * 0.5)) * 0.5;
        auto crossY = QVector3D::crossProduct(position, direction);
        crossY.normalize();
        auto rotY = QQuaternion(cos(angleY), sin(angleY) * crossY);
        rotY.normalize();
        rotateV3ByQuaternion(direction, rotY);
        auto rotU = slerp(direction, m_up, rate, 1.0f);
        if (rotU.scalar() < 1.0f)
        {
            multiplyRotation(rotU);
            rotateV3ByQuaternion(m_forward, rotU);
            rotateV3ByQuaternion(m_right, rotU);
            rotateV3ByQuaternion(m_up, rotU);
        }
    }
}

void Camera::lookAtZero(const float rate)
{
    lookAt({0.0f, 0.0f, 0.0f}, rate);
}

void Camera::lookAt(const QVector3D& point, const float rate)
{
    QMutexLocker locker(&m_mutex);
    auto direction = (m_position - point).normalized();
    auto rot = slerp(direction, m_forward, rate);

    multiplyRotation(rot);
    rotateV3ByQuaternion(m_forward, rot);
    rotateV3ByQuaternion(m_right, rot);
    rotateV3ByQuaternion(m_up, rot);
}

bool Camera::setPosition(const QVector3D& pos, const float rate)
{
    QMutexLocker locker(&m_mutex);
    if (rate >= 1.0f)
    {
        m_position = pos;
        return true;
    }
    float norm = m_position.distanceToPoint(pos);
    if (norm < 0.01f)
        return false;
    QVector3D r = (pos - m_position).normalized();
    m_position += rate * norm * r;
    return true;
}

void Camera::circleStrafing(const float amount)
{
    m_position += amount * m_right;
    lookAtZero(1.0f);
}

void Camera::roundUp(const float amount)
{
    m_position += amount * m_up;
    lookAtZero(1.0f);
}

void Camera::reset(const QVector3D& position)
{
    QMutexLocker locker(&m_mutex);
    m_position = position;
    m_rotation = QQuaternion();
    m_forward = QVector3D(0.0f, 0.0f, -1.0f);
    m_right = QVector3D(1.0f, 0.0f, 0.0f);
    m_up = QVector3D(0.0f, 1.0f, 0.0f);
}

void Camera::multiplyRotation(const QQuaternion& rot)
{
    m_rotation *= rot;
    m_rotation.normalize();
}
