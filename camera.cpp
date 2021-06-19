#include "camera.h"

static inline void rotateV3ByQuaternion(QVector3D& axis, const QQuaternion& rot)
{
    // The following calculation does not work.
    //axis = rot.rotatedVector(axis);

    auto conjugateRot = rot.conjugated();
    QQuaternion quaAxis(0.0f, axis);
    auto result = conjugateRot * quaAxis * rot;
//    if (!std::isfinite(result.x())) {
//        qDebug() << "[warning] invalid number detected";
//        return;
//    }
    axis.setX(result.x());
    axis.setY(result.y());
    axis.setZ(result.z());

    axis.normalize();

//    if (axis.x() != result.x() || axis.y() != result.y() || axis.z() != result.z())
//        qDebug() << "[warning] invalid number detected";
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
    auto rot = QQuaternion::fromAxisAndAngle(m_right, degrees);
    multiplyRotation(rot);
    rotateV3ByQuaternion(m_forward, rot);
    rotateV3ByQuaternion(m_up, rot);
}

void Camera::yaw(const float degrees)
{
    auto rot = QQuaternion::fromAxisAndAngle(m_up, degrees);
    multiplyRotation(rot);
    rotateV3ByQuaternion(m_forward, rot);
    rotateV3ByQuaternion(m_right, rot);
}

void Camera::roll(const float degrees)
{
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
    if (resetY)
        m_position.setY(-1.0f);

    auto direction = QVector3D(0.0f, 1.0f, 0.0f);
    auto cosVal = QVector3D::dotProduct(direction, m_up);
    if (cosVal > 0.999f)
        return false;
    auto angle = acos(cosVal) * rate * 0.5f;
    auto cross = QVector3D::crossProduct(direction, m_up);
    cross.normalize();
    auto rot = QQuaternion(cos(angle), sin(angle) * cross);
    rot.normalize();

    multiplyRotation(rot);
    rotateV3ByQuaternion(m_forward, rot);
    rotateV3ByQuaternion(m_right, rot);
    rotateV3ByQuaternion(m_up, rot);

    return true;
}

bool Camera::lookAtZero(const float rate)
{
    auto direction = m_position.normalized();
    auto cosVal = QVector3D::dotProduct(direction, m_forward);
    if (cosVal > 0.999f)
        return false;
    auto angle = acos(cosVal) * rate * 0.5f;
    auto cross = QVector3D::crossProduct(direction, m_forward);
    cross.normalize();
    auto rot = QQuaternion(cos(angle), sin(angle) * cross);
    rot.normalize();

    multiplyRotation(rot);
    rotateV3ByQuaternion(m_forward, rot);
    rotateV3ByQuaternion(m_right, rot);
    rotateV3ByQuaternion(m_up, rot);

    return true;
}

bool Camera::lookAt(const QVector3D& point, const float rate)
{
    //auto direction = (point - m_pos).normalized();
    auto direction = (m_position - point).normalized();
    auto cosVal = QVector3D::dotProduct(direction, m_forward);
    if (cosVal > 0.999f)
        return false;
    auto angle = acos(cosVal) * rate * 0.5f;
    auto cross = QVector3D::crossProduct(direction, m_forward);
    cross.normalize();
    auto rot = QQuaternion(cos(angle), sin(angle) * cross);
    rot.normalize();

    multiplyRotation(rot);
    rotateV3ByQuaternion(m_forward, rot);
    rotateV3ByQuaternion(m_right, rot);
    rotateV3ByQuaternion(m_up, rot);

    return true;
}

bool Camera::setPosition(const QVector3D& pos, const float rate)
{
    if (rate >= 1.0f) {
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
    auto direction = m_position.normalized();
    auto cosVal = QVector3D::dotProduct(direction, m_forward);
    if (cosVal > 1.0f) {
        // Occurs rarely with vigorous operation. Always be careful when using inner products.
        cosVal = 1.0f;
    }
    auto angle = acos(cosVal) * 0.5f;
    auto cross = QVector3D::crossProduct(direction, m_forward);
    cross.normalize();
    auto rot = QQuaternion(cos(angle), sin(angle) * cross);
    rot.normalize();

    multiplyRotation(rot);
    rotateV3ByQuaternion(m_forward, rot);
    rotateV3ByQuaternion(m_right, rot);
    rotateV3ByQuaternion(m_up, rot);
}

void Camera::reset(const QVector3D& position)
{
    m_position = position;
    m_rotation = QQuaternion();
    m_forward = QVector3D(0.0f, 0.0f, -1.0f);
    m_right = QVector3D(1.0f, 0.0f, 0.0f);
    m_up = QVector3D(0.0f, 1.0f, 0.0f);
}

void Camera::multiplyRotation(const QQuaternion& rot)
{
//    if (rot.scalar() != rot.scalar() ||
//            rot.x() != rot.x() ||
//            rot.y() != rot.y() ||
//            rot.z() != rot.z()) {
//        qDebug() << "[warning] invalid number detected"
//            << m_rotation << m_position
//            << m_forward << m_right << m_up;
//        return;
//    }
    m_rotation *= rot;
    m_rotation.normalize();
}
