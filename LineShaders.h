#pragma once

#include "bhs.h"
#include "hnn/vector3.h"
#include "hnn/octonion.h"
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>
#include <QVector3D>

using namespace hnn;

class LineShaders : private QOpenGLFunctions_4_5_Core
{
public:
    LineShaders();
    ~LineShaders();

    bool initialize();
    void paint(const QMatrix4x4& viewProjection);
    void enableGridLines(bool);
    void setLineType(const bhs::LineType);

private:
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;

    void initGridLines();
    bool m_enableGridLines;

    void appendLine(const hnn::Vector3&, const Vector3&, const QVector3D& color);
    QVector<QVector3D> m_vertex;

    static constexpr QVector3D RED = {1, 0, 0};
    static constexpr QVector3D GREEN = {0, 1, 0};
    static constexpr QVector3D BLUE = {0, 0, 1};
    static constexpr QVector3D WHITE = {1, 1, 1};

    void linesAxis();
    void linesCubeMeshes();
    void linesXZMeshes();
    void linesLongitudeAndLatitude();
    void linesQuaternionLeftIsoclinicRotation();
    void linesQuaternionRightIsoclinicRotation();
    void linesOctonionRotationYList();
    void linesOctonionRotationAll();
    void linesOctonionRotationAt(int, int, int, int, int, double y0 = 2.5);
    void linesOctonionRotation2(int, int, int, int, int);
    void linesOctonionRotationXY(int, int, int, int);
    void linesOctonionRotationYZ(int, int, int, int);
    void linesOctonionRotationZX(int, int, int, int);
    void linesLorentzTrans1();
    void linesLorentzTrans2();
    int m_ScreenX = 0;
    void drawCircle(int resolution, const Vector3& axis, const Vector3& startPoint, const QVector3D color);

};
