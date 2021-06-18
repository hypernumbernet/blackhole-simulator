#include "worldmodels.h"

static inline QVector3D rotateV3ByQuaternion(const QVector3D& axis, const QQuaternion& rot)
{
    auto conjugateRot = rot.conjugated();
    QQuaternion quaAxis(0.0f, axis);
    auto result = conjugateRot * quaAxis * rot;
    QVector3D ret(result.x(), result.y(), result.z());
    return ret;
}

WorldModels::WorldModels()
    : m_enableGridLines(true)
    , m_lineType(0)
{
    linesXZMeshes();
}

void WorldModels::appendLine(const QVector3D& start, const QVector3D& end, const QVector3D& color)
{
    m_vertex.append(start);
    m_vertex.append(color);
    m_vertex.append(end);
    m_vertex.append(color);
}

WorldModels::~WorldModels()
{
    m_vao.destroy();
}

bool WorldModels::initialize()
{
    if (!initializeOpenGLFunctions()) {
        return false;
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/world.vert")) {
        return false;
    }
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/world.frag")) {
        return false;
    }
    if (!m_program.link()) {
        return false;
    }
    if (!m_vao.create()) {
        return false;
    }
    initGridLines();
    return true;
}

void WorldModels::initGridLines()
{
    m_vao.bind();

//    auto len = m_lines.length();
//    auto sizev3d = sizeof(QVector3D);

    QOpenGLBuffer glBuf;
    glBuf.create();
    glBuf.bind();
    glBuf.allocate(m_vertex.constData(), m_vertex.length() * sizeof(QVector3D));

    quintptr offset = 0;
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, offset, 3, sizeof(QVector3D) * 2);

    offset += sizeof(QVector3D);
    m_program.enableAttributeArray(1);
    m_program.setAttributeBuffer(1, GL_FLOAT, offset, 3, sizeof(QVector3D) * 2);

    m_vao.release();
}

void WorldModels::paint(const QMatrix4x4& viewProjection)
{
    if (m_enableGridLines) {
        m_program.bind();
        m_program.setUniformValue("mvp_matrix", viewProjection);
        m_vao.bind();

        //glLineWidth(5.0f);
        glDrawArrays(GL_LINES, 0, m_vertex.length() * sizeof(QVector3D));

        m_vao.release();
        m_program.release();
    }
}

void WorldModels::enableGridLines(const bool enabled)
{
    m_enableGridLines = enabled;
}

void WorldModels::changeLineType()
{
    m_vertex.clear();
    ++m_lineType;
    if (m_lineType > 3)
        m_lineType = 0;
    switch (m_lineType) {
    case 0:
        linesXZMeshes();
        break;
    case 1:
        linesAxis();
        break;
    case 2:
        lines2Meshes();
        break;
    case 3:
        linesCubeMeshes();
        break;
    }
    initGridLines();
}

void WorldModels::linesAxis()
{
    appendLine({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, RED);
    appendLine({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, GREEN);
    appendLine({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, BLUE);
}

void WorldModels::lines2Meshes()
{
    for (int i = 0; i < 3; ++i) {
        appendLine({-1.0f, -1.0f, i - 1.0f}, {1.0f, -1.0f, i - 1.0f}, RED);
        appendLine({i - 1.0f, -1.0f, -1.0f}, {i - 1.0f, 1.0f, -1.0f}, GREEN);
        appendLine({-1.0f, i - 1.0f, -1.0f}, {-1.0f, i - 1.0f, 1.0f}, BLUE);
    }
    for (int i = 1; i < 3; ++i) {
        appendLine({i - 1.0f, -1.0f, -1.0f}, {i - 1.0f, -1.0f, 1.0f}, RED);
        appendLine({-1.0f, i - 1.0f, -1.0f}, {1.0f, i - 1.0f, -1.0f}, GREEN);
        appendLine({-1.0f, -1.0f, i - 1.0f}, {-1.0f, 1.0f, i - 1.0f}, BLUE);
    }
}

void WorldModels::linesXZMeshes()
{
    for (int i = -10; i <= 10; ++i) {
        appendLine({-10, 0.0f, (float)i}, {10, 0.0f, (float)i}, RED);
        appendLine({(float)i, 0.0f, -10}, {(float)i, 0.0f, 10}, BLUE);
    }
    appendLine({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, GREEN);
}

void WorldModels::drawCircle(const int resolution, const QVector3D& axis, const QVector3D& startPoint, const QVector3D color)
{
    float degree = 360.0f / (float)resolution;
    QVector3D prev = startPoint;
    for (int i = 0; i <= resolution; ++i) {
        QQuaternion rot = QQuaternion::fromAxisAndAngle(axis, degree * (float)i);
        QVector3D v = rotateV3ByQuaternion(startPoint, rot);
        if (i > 0)
            appendLine(prev, v, color);
        prev = v;
    }
}

void WorldModels::linesCubeMeshes()
{
    int resolution = 36;
    float angle = 10.0f;

    const QVector3D axis_y(0.0f, 1.0f, 0.0f);
    const QVector3D axis_x(1.0f, 0.0f, 0.0f);

    int jmax = floor(resolution / 2) - 1;
    QQuaternion rot_y = QQuaternion::fromAxisAndAngle(axis_y, angle);
    QVector3D meridian_start = axis_x;
    drawCircle(resolution, meridian_start, axis_y, RED);
    for (int j = 0; j < jmax; ++j) {
        meridian_start = rotateV3ByQuaternion(meridian_start, rot_y);
        drawCircle(resolution, meridian_start, axis_y, RED);
    }

    const QVector3D axis_z(0.0f, 0.0f, 1.0f);
    int xzmax = floor(resolution / 4);
    for (int j = 0; j < xzmax; ++j) {
        QVector3D v = axis_z;
        if (j > 0) {
            QQuaternion rot_zy = QQuaternion::fromAxisAndAngle(axis_x, angle * (float)j);
            v = rotateV3ByQuaternion(axis_z, rot_zy);
        }
        drawCircle(resolution, axis_y, v, BLUE);
        drawCircle(resolution, axis_y, {v.x(), -v.y(), v.z()}, BLUE);
    }

    appendLine({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, RED);
    appendLine({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, GREEN);
    appendLine({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, BLUE);

}
