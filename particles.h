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
    void setNumberOfParticles(int num);

    static constexpr float GRAVITATIONAL_CONSTANT = 6.6743e-11f; // 2018 CODATA
    static constexpr float SPEED_OF_LIGHT = 2.99792458e+8f;

    float* location;
    float timePerFrame; // physical time

private:
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;

    float pointSizeScale;
    float pointSize;
    int initHeight;
    int numberOfParticles;
};
