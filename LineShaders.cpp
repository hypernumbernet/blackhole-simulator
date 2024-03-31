#include "LineShaders.h"
#include "bhs.h"
#include "hnn/octonion.h"
#include "hnn/splitoctonion.h"
#include "engine/Relativity1/CalculationRelativity1.h"
#include <QOpenGLBuffer>

LineShaders::LineShaders()
    : m_enableGridLines(true)
{
    linesXZMeshes();
}

void LineShaders::appendLine(const Vector3& start, const Vector3& end, const QVector3D& color)
{
    m_vertex.append(QVector3D(float(start.x()), float(start.y()), float(start.z())));
    m_vertex.append(color);
    m_vertex.append(QVector3D(float(end.x()), float(end.y()), float(end.z())));
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
    glBuf.allocate(m_vertex.constData(), m_vertex.length() * int(sizeof(QVector3D)));

    quintptr offset = 0;
    m_program.enableAttributeArray(0);
    m_program.setAttributeBuffer(0, GL_FLOAT, offset, 3, int(sizeof(QVector3D)) * 2);

    offset += sizeof(QVector3D);
    m_program.enableAttributeArray(1);
    m_program.setAttributeBuffer(1, GL_FLOAT, offset, 3, int(sizeof(QVector3D)) * 2);

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
        glDrawArrays(GL_LINES, 0, m_vertex.length() * int(sizeof(QVector3D)));

        m_vao.release();
        m_program.release();
    }
}

void LineShaders::enableGridLines(const bool enabled)
{
    m_enableGridLines = enabled;
}

void LineShaders::setLineType(const bhs::LineType index)
{
    m_vertex.clear();

    switch (index)
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
    case bhs::LineType::Quaternion3DRotation:
        linesQuaternion3DRotation();
        break;
    case bhs::LineType::QuaternionLeftIsoclinicRotation:
        linesQuaternionLeftIsoclinicRotation();
        break;
    case bhs::LineType::QuaternionRightIsoclinicRotation:
        linesQuaternionRightIsoclinicRotation();
        break;
    case bhs::LineType::OctonionRotationLeft:
        linesOctonionRotationXY(1,2,4,7);
        break;
    case bhs::LineType::OctonionRotationRight:
        linesOctonionRotationXY(1,2,5,6);
        break;
    case bhs::LineType::OctonionRotationCloseXY:
        linesOctonionRotationXY(1,2,4,5);
        break;
    case bhs::LineType::OctonionRotationOpenXY:
        linesOctonionRotationXY(1,2,3,4);
        break;
    case bhs::LineType::OctonionRotationOpenYZ:
        linesOctonionRotationYZ(1,2,4,5);
        break;
    case bhs::LineType::OctonionRotationOpenZX:
        linesOctonionRotationZX(2,5,6,7);
        break;
    case bhs::LineType::OctonionRotationYList:
        linesOctonionRotationYList();
        break;
    case bhs::LineType::OctonionRotationAll:
        linesOctonionRotationAll();
        break;
    case bhs::LineType::SplitOctonionRotation:
        linesSplitOctonionRotation();
        break;
    case bhs::LineType::LorentzTrans1:
        linesLorentzTrans1();
        break;
    case bhs::LineType::LorentzTrans2:
        linesLorentzTrans2();
        break;
    }
    initGridLines();
}

void LineShaders::linesAxis()
{
    appendLine({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, RED);
    appendLine({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, GREEN);
    appendLine({0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, BLUE);
}

void LineShaders::linesCubeMeshes()
{
    for (int i = 0; i < 3; ++i)
    {
        appendLine({-1.0, -1.0, i - 1.0}, {1.0, -1.0, i - 1.0}, RED);
        appendLine({i - 1.0, -1.0, -1.0}, {i - 1.0, 1.0, -1.0}, GREEN);
        appendLine({-1.0, i - 1.0, -1.0}, {-1.0, i - 1.0, 1.0}, BLUE);
    }
    for (int i = 1; i < 3; ++i)
    {
        appendLine({i - 1.0, -1.0, -1.0}, {i - 1.0, -1.0, 1.0}, RED);
        appendLine({-1.0, i - 1.0, -1.0}, {1.0, i - 1.0, -1.0}, GREEN);
        appendLine({-1.0, -1.0, i - 1.0}, {-1.0, 1.0, i - 1.0}, BLUE);
    }
}

void LineShaders::linesXZMeshes()
{
    for (int i = -10; i <= 10; ++i)
    {
        appendLine({-10.0, 0.0, double(i)}, {10.0, 0.0, double(i)}, RED);
        appendLine({double(i), 0.0, -10.0}, {double(i), 0.0, 10.0}, BLUE);
    }
    appendLine({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, GREEN);
}

void LineShaders::drawCircle(
        const int resolution, const Vector3& axis,
        const Vector3& startPoint, const QVector3D color)
{
    double angle = hnn::PI * 2.0 / (double)resolution;
    Vector3 prev = startPoint;
    for (int i = 0; i <= resolution; ++i)
    {
        auto rot = Quaternion::rotation(axis, angle * (double)i);
        Vector3 v = startPoint;
        Quaternion::rotate(v, rot);
        if (i > 0)
            appendLine(prev, v, color);
        prev = v;
    }
}

void LineShaders::linesLongitudeAndLatitude()
{
    static const int resolution = 36;
    static const double angle = 2. * hnn::PI / double(resolution);

    const Vector3 axis_y(0.0, 1.0, 0.0);
    const Vector3 axis_x(1.0, 0.0, 0.0);

    int jmax = floor(resolution / 2) - 1;
    auto rot_y = Quaternion::rotation(axis_y, angle);
    Vector3 meridian_start = axis_x;
    drawCircle(resolution, meridian_start, axis_y, RED);
    for (int j = 0; j < jmax; ++j)
    {
        Quaternion::rotate(meridian_start, rot_y);
        drawCircle(resolution, meridian_start, axis_y, RED);
    }

    const Vector3 axis_z(0.0, 0.0, 1.0);
    int xzmax = floor(resolution / 4);
    for (int j = 0; j < xzmax; ++j)
    {
        Vector3 v = axis_z;
        if (j > 0)
        {
            Quaternion rot_zy = Quaternion::rotation(axis_x, angle * (double)j);
            v = axis_z;
            Quaternion::rotate(v, rot_zy);
        }
        drawCircle(resolution, axis_y, v, BLUE);
        drawCircle(resolution, axis_y, {v.x(), -v.y(), v.z()}, BLUE);
    }

    appendLine({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, RED);
    appendLine({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, GREEN);
    appendLine({0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, BLUE);
}

void LineShaders::linesQuaternion3DRotation()
{
    static const int resolution = 50;
    Vector3 a(-2,2,3);
    a.normalize();
    Vector3 b(3,2,-1);
    b.normalize();
    double theta = acos(a.dot(b));
    Vector3 w = a.cross(b);
    w.normalize();
    double angle = theta / double(resolution);
    Quaternion h = Quaternion::rotation(w, angle);
    Vector3 f = h.rotate(a);

    appendLine({0.0, 0.0, 0.0}, a, {float(a.x()), float(a.y()), float(a.z())});
    appendLine({0.0, 0.0, 0.0}, b, {float(b.x()), float(b.y()), float(b.z())});
    for (int i = 1; i < resolution; ++i)
    {
        appendLine({0.0, 0.0, 0.0}, f, {float(f.x()), float(f.y()), float(f.z())});
        f = h.rotate(f);
    }

    appendLine({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, RED);
    appendLine({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, GREEN);
    appendLine({0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, BLUE);
}

void LineShaders::linesQuaternionLeftIsoclinicRotation()
{
    static const int resolution = 180;
    static const double angle = 2. * degreeToRadian(360.0 / double(resolution));

    const auto rotationY = Quaternion::rotation({0,1,0}, angle);
    for (int j = -4; j <= 4; ++j)
    {
        if (j == 0)
            continue;
        auto start = Quaternion::rotation({1,0,0}, j * 0.4);
        auto end = rotationY * start;
        for (int i = 0; i < resolution; ++i)
        {
            appendLine(start.lnV3() / 3.14, end.lnV3() / 3.14, WHITE);
            start = end;
            end = rotationY * start;
        }
    }
    appendLine({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, RED);
    appendLine({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, GREEN);
    appendLine({0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, BLUE);
}

void LineShaders::linesQuaternionRightIsoclinicRotation()
{
    static const int resolution = 180;
    static const double angle = 2. * degreeToRadian(360.0 / double(resolution));

    const auto rotationY = Quaternion::rotation({0,1,0}, angle);
    for (int j = -4; j <= 4; ++j)
    {
        if (j == 0)
            continue;
        auto start = Quaternion::rotation({1,0,0}, j * 0.4);
        auto end = start * rotationY;
        for (int i = 0; i < resolution; ++i)
        {
            appendLine(start.lnV3() / 3.14, end.lnV3() / 3.14, WHITE);
            start = end;
            end = start * rotationY;
        }
    }
    appendLine({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, RED);
    appendLine({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, GREEN);
    appendLine({0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, BLUE);
}

void LineShaders::linesOctonionRotationYList()
{
    m_ScreenX = 0;
    for (int i1 = 1; i1 <= 4; ++i1)
        for (int i2 = i1 + 1; i2 <= 5; ++i2)
            for (int i3 = i2 + 1; i3 <= 6; ++i3)
                for (int i4 = i3 + 1; i4 <= 7; ++i4)
                {
                    qDebug() << i1 << i2 << i3 << i4;
                    linesOctonionRotationAt(i1, i2, i3, i4, 0, 0.0);
                }
}

void LineShaders::linesOctonionRotationAll()
{
    for (int i5 = 0; i5 < 6; ++i5)
    {
        m_ScreenX = 0;
        for (int i1 = 1; i1 <= 4; ++i1)
            for (int i2 = i1 + 1; i2 <= 5; ++i2)
                for (int i3 = i2 + 1; i3 <= 6; ++i3)
                    for (int i4 = i3 + 1; i4 <= 7; ++i4)
                    {
                        qDebug() << i1 << i2 << i3 << i4;
                        linesOctonionRotationAt(i1, i2, i3, i4, i5);
                    }
    }
}

void LineShaders::linesOctonionRotationAt(int w, int x, int y, int z, int pole, double y0)
{
    static const int resolution = 72;
    static const double angle = degreeToRadian(360.0 / double(resolution));
    static const double scale = 0.1;
    static const double slideScale = 1.0;
    double slideX = (double(m_ScreenX / 5) - 3.0) * slideScale;
    double slideY = (double(pole) - y0) * slideScale;
    double slideZ = (double(m_ScreenX % 5) - 2.0) * slideScale;
    QVector3D color((float)bhs::rand0to1(),(float)bhs::rand0to1(),(float)bhs::rand0to1());
    color.normalize();

    Octonion origin(0);
    Octonion x90(0);
    Octonion y90(0);
    origin[w] = 1;
    switch (pole) {
    default:
    case 0: x90[x] = 1; y90[y] = 1; break;
    case 1: x90[z] = 1; y90[y] = 1; break;
    case 2: x90[x] = 1; y90[z] = 1; break;
    case 3: x90[y] = 1; y90[z] = 1; break;
    case 4: x90[y] = 1; y90[x] = 1; break;
    case 5: x90[z] = 1; y90[x] = 1; break;
    }
    auto poleX(origin.cross(x90));
    auto poleY(origin.cross(y90));
    Octonion rotationY(Octonion::rotation(poleY, angle));

    for (int j = -4; j <= 4; ++j)
    {
        Octonion rotationX(Octonion::rotation(poleX, j * 0.2));
        auto startX = rotationX.conjugated() * origin * rotationX;
        Quaternion start(startX[w],startX[x],startX[y],startX[z]);
        auto startY = startX;
        auto endY = rotationY.conjugated() * startX * rotationY;
        Quaternion end(endY[w],endY[x],endY[y],endY[z]);
        for (int i = 0; i < resolution; ++i)
        {
            auto st = start.lnV3() * scale;
            st.setX(st.x() + slideX);
            st.setY(st.y() + slideY);
            st.setZ(st.z() + slideZ);
            auto ed = end.lnV3() * scale;
            ed.setX(ed.x() + slideX);
            ed.setY(ed.y() + slideY);
            ed.setZ(ed.z() + slideZ);
            appendLine(st, ed, color);
            startY = endY;
            start = end;
            endY = rotationY.conjugated() * startY * rotationY;
            end = {endY[w],endY[x],endY[y],endY[z]};
        }
    }
    ++m_ScreenX;
}

void LineShaders::linesOctonionRotation2(int w, int x, int y, int z, int pole)
{
    static const int resolution = 180;
    static const double angle = degreeToRadian(360.0 / double(resolution));
    static const double scale = 1.0 / 3.14;

    Octonion origin(0);
    Octonion x90(0);
    Octonion y90(0);
    origin[w] = 1;
    switch (pole) {
    default:
    case 0: x90[x] = 1; y90[y] = 1; break;
    case 1: x90[z] = 1; y90[y] = 1; break;
    case 2: x90[x] = 1; y90[z] = 1; break;
    case 3: x90[y] = 1; y90[z] = 1; break;
    case 4: x90[y] = 1; y90[x] = 1; break;
    case 5: x90[z] = 1; y90[x] = 1; break;
    }
    auto poleX(origin.cross(x90));
    auto poleY(origin.cross(y90));
    Octonion rotationY(Octonion::rotation(poleY, angle));

    for (int j = -4; j <= 4; ++j)
    {
        if (j == 0)
            continue;
        Octonion rotationX(Octonion::rotation(poleX, j * 0.2));
        auto startX = rotationX.conjugated() * origin * rotationX;
        Quaternion start(startX[w],startX[x],startX[y],startX[z]);
        auto startY = startX;
        auto endY = rotationY.conjugated() * startX * rotationY;
        Quaternion end(endY[w],endY[x],endY[y],endY[z]);
        for (int i = 0; i < resolution; ++i)
        {
            auto st = start.lnV3() * scale;
            auto ed = end.lnV3() * scale;
            appendLine(st, ed, WHITE);
            startY = endY;
            start = end;
            endY = rotationY.conjugated() * startY * rotationY;
            end = {endY[w],endY[x],endY[y],endY[z]};
        }
    }
}

void LineShaders::linesOctonionRotationXY(int w, int x, int y, int z)
{
    linesOctonionRotation2(w,x,y,z,0);
    linesOctonionRotation2(w,x,y,z,4);
    appendLine({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, RED);
    appendLine({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, GREEN);
    appendLine({0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, BLUE);
}

void LineShaders::linesOctonionRotationYZ(int w, int x, int y, int z)
{
    linesOctonionRotation2(w,x,y,z,1);
    linesOctonionRotation2(w,x,y,z,3);
    appendLine({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, RED);
    appendLine({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, GREEN);
    appendLine({0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, BLUE);
}

void LineShaders::linesOctonionRotationZX(int w, int x, int y, int z)
{
    linesOctonionRotation2(w,x,y,z,2);
    linesOctonionRotation2(w,x,y,z,5);
    appendLine({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, RED);
    appendLine({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, GREEN);
    appendLine({0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, BLUE);
}

void LineShaders::linesSplitOctonionRotation()
{
    int resolution = 180;
    double angle = degreeToRadian(360.0 / double(resolution));
    double scale = 1.0 / 3.14;


    appendLine({0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, RED);
    appendLine({0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, GREEN);
    appendLine({0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, BLUE);
}

void LineShaders::linesLorentzTrans1()
{
    static double rateXZ = 0.07;
    static double rateY = 0.05;
    QGenericMatrix<4, 4, double> matrix;
    Vector3 speed(0., 0., 0.);
    double speedOfLightInv = 1. / SPEED_OF_LIGHT;
    QGenericMatrix<1, 4, double> spacetimeOrg;
    spacetimeOrg(0, 0) = 1.;
    spacetimeOrg(1, 0) = 0.;
    spacetimeOrg(2, 0) = 0.;
    spacetimeOrg(3, 0) = 0.;
    QGenericMatrix<1, 4, double> spacetimeA, spacetimeB;

    for (int k = -8; k <= 8; k += 2)
    {
        for (int j = -8; j <= 8; j += 2)
        {
            for (int i = -8; i <= 8; i += 2)
            {
                speed.set(SPEED_OF_LIGHT * rateXZ * (double)k, SPEED_OF_LIGHT * rateY * (double)i, SPEED_OF_LIGHT * rateXZ * (double)j);
                CalculationRelativity1::lorentzTransformation(matrix, speed, speedOfLightInv);
                spacetimeB = matrix * spacetimeOrg;
                if (i > -8)
                {
                    double ax = spacetimeA(1, 0);
                    double ay = spacetimeA(2, 0);
                    double az = spacetimeA(3, 0);
                    double bx = spacetimeB(1, 0);
                    double by = spacetimeB(2, 0);
                    double bz = spacetimeB(3, 0);
                    appendLine({ax, ay, az}, {bx, by, bz}, GREEN);
                }
                spacetimeA = spacetimeB;
            }
        }
    }
    for (int k = -8; k <= 8; k += 2)
    {
        for (int j = -8; j <= 8; j += 2)
        {
            speed.set(SPEED_OF_LIGHT * rateXZ * (double)j, 0., SPEED_OF_LIGHT * rateXZ * (double)k);
            CalculationRelativity1::lorentzTransformation(matrix, speed, speedOfLightInv);
            spacetimeB = matrix * spacetimeOrg;
            if (j > -8)
            {
                double ax = spacetimeA(1, 0);
                double ay = spacetimeA(2, 0);
                double az = spacetimeA(3, 0);
                double bx = spacetimeB(1, 0);
                double by = spacetimeB(2, 0);
                double bz = spacetimeB(3, 0);
                appendLine({ax, ay, az}, {bx, by, bz}, RED);
            }
            spacetimeA = spacetimeB;
        }
    }
    for (int k = -8; k <= 8; k += 2)
    {
        for (int j = -8; j <= 8; j += 2)
        {
            speed.set(SPEED_OF_LIGHT * rateXZ * (double)k, 0., SPEED_OF_LIGHT * rateXZ * (double)j);
            CalculationRelativity1::lorentzTransformation(matrix, speed, speedOfLightInv);
            spacetimeB = matrix * spacetimeOrg;
            if (j > -8)
            {
                double ax = spacetimeA(1, 0);
                double ay = spacetimeA(2, 0);
                double az = spacetimeA(3, 0);
                double bx = spacetimeB(1, 0);
                double by = spacetimeB(2, 0);
                double bz = spacetimeB(3, 0);
                appendLine({ax, ay, az}, {bx, by, bz}, BLUE);
            }
            spacetimeA = spacetimeB;
        }
    }
}

void LineShaders::linesLorentzTrans2()
{
    static double rateXZ = 0.07;
    static double rateY = 0.05;
    double speedOfLightInv = 1. / SPEED_OF_LIGHT;
    Vector3 speed(0., 0., 0.);
    Spacetime spacetimeA(0.), spacetimeB(0.);

    for (int k = -8; k <= 8; k += 2)
    {
        for (int j = -8; j <= 8; j += 2)
        {
            for (int i = -8; i <= 8; i += 2)
            {
                speed.set(SPEED_OF_LIGHT * rateXZ * (double)k, SPEED_OF_LIGHT * rateY * (double)i, SPEED_OF_LIGHT * rateXZ * (double)j);
                spacetimeB = Spacetime::identity();
                spacetimeB.lorentzTransformation(speed, speedOfLightInv);
                if (i > -8)
                {
                    appendLine({spacetimeA.x(), spacetimeA.y(), spacetimeA.z()}, {spacetimeB.x(), spacetimeB.y(), spacetimeB.z()}, GREEN);
                }
                spacetimeA = spacetimeB;
            }
        }
    }
    for (int k = -8; k <= 8; k += 2)
    {
        for (int j = -8; j <= 8; j += 2)
        {
            speed.set(SPEED_OF_LIGHT * rateXZ * (double)j, 0., SPEED_OF_LIGHT * rateXZ * (double)k);
            spacetimeB = Spacetime::identity();
            spacetimeB.lorentzTransformation(speed, speedOfLightInv);
            if (j > -8)
            {
                appendLine({spacetimeA.x(), spacetimeA.y(), spacetimeA.z()}, {spacetimeB.x(), spacetimeB.y(), spacetimeB.z()}, RED);
            }
            spacetimeA = spacetimeB;
        }
    }
    for (int k = -8; k <= 8; k += 2)
    {
        for (int j = -8; j <= 8; j += 2)
        {
            speed.set(SPEED_OF_LIGHT * rateXZ * (double)k, 0., SPEED_OF_LIGHT * rateXZ * (double)j);
            spacetimeB = Spacetime::identity();
            spacetimeB.lorentzTransformation(speed, speedOfLightInv);
            if (j > -8)
            {
                appendLine({spacetimeA.x(), spacetimeA.y(), spacetimeA.z()}, {spacetimeB.x(), spacetimeB.y(), spacetimeB.z()}, BLUE);
            }
            spacetimeA = spacetimeB;
        }
    }
}
