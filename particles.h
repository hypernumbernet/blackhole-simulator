#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class Particles : public QOpenGLFunctions_3_3_Core
{
public:
    Particles(int screenHeight);
    ~Particles();

    bool initialize();
    void paint(QMatrix4x4 viewProjection);
    void resize(int height);

    void updateParticles();
    float particlePosition[3000];

private:
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;

    float pointSizeScale;
    float pointSize;
    int initHeight;
};
