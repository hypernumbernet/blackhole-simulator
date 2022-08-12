#include "lineshaders.h"

LineShaders::LineShaders()
    : m_enableGridLines(true)
{
    linesXZMeshes();
}

void LineShaders::appendLine(const QVector3D& start, const QVector3D& end, const QVector3D& color)
{
    m_vertex.append(start);
    m_vertex.append(color);
    m_vertex.append(end);
    m_vertex.append(color);
}

void LineShaders::appendLineHnn(const Vector3<float>& start, const Vector3<float>& end, const QVector3D& color)
{
    m_vertex.append(QVector3D(start.x(), start.y(), start.z()));
    m_vertex.append(color);
    m_vertex.append(QVector3D(end.x(), end.y(), end.z()));
    m_vertex.append(color);
}

LineShaders::~LineShaders()
{
    m_vao.destroy();
}

bool LineShaders::initialize()
{
    if (!initializeOpenGLFunctions())
        return false;

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/world.vert"))
        return false;

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/world.frag"))
        return false;

    if (!m_program.link())
        return false;

    if (!m_vao.create())
        return false;

    initGridLines();
    return true;
}

void LineShaders::initGridLines()
{
    m_vao.bind();

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

void LineShaders::paint(const QMatrix4x4& viewProjection)
{
    if (m_enableGridLines)
    {
        m_program.bind();
        m_program.setUniformValue("mvp_matrix", viewProjection);
        m_vao.bind();

        //glLineWidth(5.0f);
        glDrawArrays(GL_LINES, 0, m_vertex.length() * sizeof(QVector3D));

        m_vao.release();
        m_program.release();
    }
}

void LineShaders::enableGridLines(const bool enabled)
{
    m_enableGridLines = enabled;
}

void LineShaders::setLineType(const int index)
{
    m_vertex.clear();

    switch (static_cast<bhs::LineType>(index))
    {
    case bhs::LineType::XZMeshes:
        linesXZMeshes();
        break;
    case bhs::LineType::Axis:
        linesAxis();
        break;
    case bhs::LineType::CubeMeshes:
        linesCubeMeshes();
        break;
    case bhs::LineType::LongitudeAndLatitude:
        linesLongitudeAndLatitude();
        break;
    case bhs::LineType::QuaternionLogarithm:
        linesQuaternionLogarithm();
        break;
    }
    initGridLines();
}

void LineShaders::linesAxis()
{
    appendLine({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, RED);
    appendLine({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, GREEN);
    appendLine({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, BLUE);
}

void LineShaders::linesCubeMeshes()
{
    for (int i = 0; i < 3; ++i)
    {
        appendLine({-1.0f, -1.0f, i - 1.0f}, {1.0f, -1.0f, i - 1.0f}, RED);
        appendLine({i - 1.0f, -1.0f, -1.0f}, {i - 1.0f, 1.0f, -1.0f}, GREEN);
        appendLine({-1.0f, i - 1.0f, -1.0f}, {-1.0f, i - 1.0f, 1.0f}, BLUE);
    }
    for (int i = 1; i < 3; ++i)
    {
        appendLine({i - 1.0f, -1.0f, -1.0f}, {i - 1.0f, -1.0f, 1.0f}, RED);
        appendLine({-1.0f, i - 1.0f, -1.0f}, {1.0f, i - 1.0f, -1.0f}, GREEN);
        appendLine({-1.0f, -1.0f, i - 1.0f}, {-1.0f, 1.0f, i - 1.0f}, BLUE);
    }
}

void LineShaders::linesXZMeshes()
{
    for (int i = -10; i <= 10; ++i)
    {
        appendLine({-10, 0.0f, (float)i}, {10, 0.0f, (float)i}, RED);
        appendLine({(float)i, 0.0f, -10}, {(float)i, 0.0f, 10}, BLUE);
    }
    appendLine({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, GREEN);
}

void LineShaders::drawCircle(const int resolution, const QVector3D& axis, const QVector3D& startPoint, const QVector3D color)
{
    float degree = 360.0f / (float)resolution;
    QVector3D prev = startPoint;
    for (int i = 0; i <= resolution; ++i)
    {
        QQuaternion rot = QQuaternion::fromAxisAndAngle(axis, degree * (float)i);
        QVector3D v = startPoint;
        Camera::rotateV3ByQuaternion(v, rot);
        if (i > 0)
            appendLine(prev, v, color);
        prev = v;
    }
}

void LineShaders::linesLongitudeAndLatitude()
{
    static const int resolution = 36;
    static const float angle = 360.0f / float(resolution);

    const QVector3D axis_y(0.0f, 1.0f, 0.0f);
    const QVector3D axis_x(1.0f, 0.0f, 0.0f);

    int jmax = floor(resolution / 2) - 1;
    QQuaternion rot_y = QQuaternion::fromAxisAndAngle(axis_y, angle);
    QVector3D meridian_start = axis_x;
    drawCircle(resolution, meridian_start, axis_y, RED);
    for (int j = 0; j < jmax; ++j)
    {
        Camera::rotateV3ByQuaternion(meridian_start, rot_y);
        drawCircle(resolution, meridian_start, axis_y, RED);
    }

    const QVector3D axis_z(0.0f, 0.0f, 1.0f);
    int xzmax = floor(resolution / 4);
    for (int j = 0; j < xzmax; ++j)
    {
        QVector3D v = axis_z;
        if (j > 0)
        {
            QQuaternion rot_zy = QQuaternion::fromAxisAndAngle(axis_x, angle * (float)j);
            v = axis_z;
            Camera::rotateV3ByQuaternion(v, rot_zy);
        }
        drawCircle(resolution, axis_y, v, BLUE);
        drawCircle(resolution, axis_y, {v.x(), -v.y(), v.z()}, BLUE);
    }

    appendLine({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, RED);
    appendLine({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, GREEN);
    appendLine({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, BLUE);
}

void LineShaders::linesQuaternionLogarithm()
{
    static const int resolution = 72;
    static const float angle = degreeToRadian(360.0f / float(resolution));

//    const auto angleX = Quaternion<float>::exp(angle, 0.0f, 0.0f);
//    for (int j = -16; j <= 16; j += 2)
//    {
//        auto start = Quaternion<float>::exp(0.0f, 0.0f, angle * j);
//        auto end = angleX * start;
//        for (int i = 0; i < resolution; ++i)
//        {
//            appendLineHnn(start.lnV3(), end.lnV3(), {1,1,1});
//            start = end;
//            end = angleX * start;
//        }
//    }

    //    const auto rotX = Quaternion<float>::makeRotation(1.0f, 0.0f, 0.0f, angle * 0.5f);
    //    for (int j = 0; j < 3; ++j)
    //    {
    //        auto start = Quaternion<float>::exp(0.0f, angle * j, 0.0f);
    //        auto end = start.rotated8(rotX);
    //        for (int i = 0; i < resolution; ++i)
    //        {
    //            appendLineHnn(start.lnV3() / pi, end.lnV3() / pi, GREEN);
    //            start = end;
    //            end = start.rotated8(rotX);
    //        }
    //    }

    //    auto cross = origin.cross7V3(angleX);
    //    cross.normalize();
    //    for (int j = 0; j < 2; ++j)
    //    {
    //        auto start = Quaternion<float>::exp(0.0f, angle * j, 0.0f);
    //        auto end = start.rotMove(cross, angle);
    //        for (int i = 0; i < resolution; ++i)
    //        {
    //            appendLineHnn(start.lnV3() / pi, end.lnV3() / pi, BLUE);
    //            start = end;
    //            end = start.rotMove(cross, angle);
    //        }
    //    }

    Octonion<float> origin(0,0,0,0,1,0,0,0);
    Octonion<float> x90(0,0,0,0,0,1,0,0);
    Octonion<float> y90(0,0,0,0,0,0,1,0);
    Octonion<float> z90(0,0,0,0,0,0,0,1);
    auto poleX(x90 * origin);
    auto poleY(y90 * origin);
    auto poleZ(z90 * origin);
    qDebug(poleX.toString().c_str());
    qDebug(poleY.toString().c_str());
    qDebug(poleZ.toString().c_str());

    auto rotationX(poleX * sin(angle));
    rotationX.setRe(cos(angle));
    auto rotationY(poleY * sin(angle));
    rotationY.setRe(cos(angle));

    auto startX(origin);
    //startX = rotationX.conjugated() * startX * rotationX;
    for (int j = 0; j < (resolution * 0.5); ++j)
    {
        auto startY(startX);
        Quaternion<float> start(startY.i4(),startY.i5(),startY.i6(),startY.i7());
        auto endY = rotationY.conjugated() * startY * rotationY;
        Quaternion<float> end(endY.i4(),endY.i5(),endY.i6(),endY.i7());
        for (int i = 0; i < (resolution * 0.1); ++i)
        {
            appendLineHnn(start.lnV3(), end.lnV3(), {0,1,0});
            startY = endY;
            start = end;
            endY = rotationY.conjugated() * startY * rotationY;
            end = {endY.i4(),endY.i5(),endY.i6(),endY.i7()};
        }
        startX = rotationX.conjugated() * startX * rotationX;
    }

    Octonion<double> originL(0,0,0,0,1,0,0,0);
    Octonion<double> x90L(0,1,0,0,0,0,0,0);
    Octonion<double> y90L(0,0,1,0,0,0,0,0);
    Octonion<double> z90L(0,0,0,1,0,0,0,0);
    auto poleXL(x90L * originL);
    auto poleYL(y90L * originL);
    auto poleZL(z90L * originL);
    qDebug(poleXL.toString().c_str());
    qDebug(poleYL.toString().c_str());
    qDebug(poleZL.toString().c_str());

    auto rotationXL(poleXL * sin(angle));
    rotationXL.setRe(cos(angle));
    auto rotationYL(poleYL * sin(angle));
    rotationYL.setRe(cos(angle));
    auto rotationZL(poleZL * sin(angle));
    rotationZL.setRe(cos(angle));

    auto startXL(originL);
    startXL = rotationXL.conjugated() * startXL * rotationXL;
    for (int j = 0; j <= 8; j += 2)
    {
        auto startY(startXL);
        Quaternion<double> start(startY.i4(),startY.i1(),startY.i2(),startY.i3());
        auto endY = rotationYL.conjugated() * startY * rotationYL;
        Quaternion<double> end(endY.i4(),endY.i1(),endY.i2(),endY.i3());
        for (int i = 0; i < (resolution * 0.6); ++i)
        {
            //appendLineHnn(start.lnV3(), end.lnV3(), {1,0,0});
            Vector3<double> startLn(start.lnV3());
            Vector3<double> endLn(end.lnV3());
            //appendLineHnn({float(startLn.x()),float(startLn.y()),float(startLn.z())},{float(endLn.x()),float(endLn.y()),float(endLn.z())}, {1,0,0});
            startY.set(endY);
            start.set(end);
            //startY = {0,start.i1(),start.i2(),start.i3(),start.re(),0,0,0};
            endY = rotationYL.conjugated() * startY * rotationYL;
            end = {endY.i4(),endY.i1(),endY.i2(),endY.i3()};
        }
        startXL = rotationXL.conjugated() * startXL * rotationXL;
    }

}
