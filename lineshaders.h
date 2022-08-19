#pragma once

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector>

#include "global.h"
#include "camera.h"

class LineShaders : private QOpenGLFunctions_4_5_Core
{
public:
    LineShaders();
    ~LineShaders();

    bool initialize();
    void paint(const QMatrix4x4& viewProjection);
    void enableGridLines(bool);
    void setLineType(int);

private:
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;

    void initGridLines();
    bool m_enableGridLines;

    void appendLine(const Vector3<double>&, const Vector3<double>&, const QVector3D& color);
    QVector<QVector3D> m_vertex;

    static constexpr QVector3D RED = {1.0f, 0.0f, 0.0f};
    static constexpr QVector3D GREEN = {0.0f, 1.0f, 0.0f};
    static constexpr QVector3D BLUE = {0.0f, 0.0f, 1.0f};

    void linesAxis();
    void linesCubeMeshes();
    void linesXZMeshes();
    void linesLongitudeAndLatitude();
    void linesQuaternionS3Rotation();
    void linesOctonionS3Rotation();
    void linesOctonionRotationAt(int, int, int, int);
    int m_ScreenX = 0;
    void drawCircle(int resolution, const Vector3<double>& axis, const Vector3<double>& startPoint, const QVector3D color);

};
