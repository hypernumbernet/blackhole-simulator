#pragma once

#include "camera.h"

#include <math.h>

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector>

class WorldModels : private QOpenGLFunctions_4_5_Core
{
public:
    WorldModels();
    ~WorldModels();

    bool initialize();
    void paint(const QMatrix4x4& viewProjection);
    void enableGridLines(bool);
    void changeLineType();

private:
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;

    void initGridLines();
    bool m_enableGridLines;

    int m_lineType;

    void appendLine(const QVector3D&, const QVector3D&, const QVector3D& color);
    QVector<QVector3D> m_vertex;

    static constexpr QVector3D RED = {1.0f, 0.0f, 0.0f};
    static constexpr QVector3D GREEN = {0.0f, 1.0f, 0.0f};
    static constexpr QVector3D BLUE = {0.0f, 0.0f, 1.0f};

    void linesAxis();
    void lines2Meshes();
    void linesXZMeshes();
    void linesCubeMeshes();
    void drawCircle(int resolution, const QVector3D& axis, const QVector3D& startPoint, const QVector3D color);
};
