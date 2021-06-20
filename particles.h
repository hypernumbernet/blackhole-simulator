#pragma once

#include "g3dmassdiffnbe.h"
#include "g3dmassintegralnbe.h"

#include <math.h>

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class Particles : private QOpenGLFunctions_4_5_Core
{
public:
    Particles(UpdateUi*);
    ~Particles();

    bool initialize(int screenHeight);
    void paint(const QMatrix4x4& viewProjection);
    void resize(int height);
    void updateParticles();
    void selectNBodyEngine(const bhs::SimCondition&);
    void reset(const bhs::SimCondition&);
    void setModelScale(float);
    void setModelScaleRatio(float);

private:
    UpdateUi* const m_updateUi;

    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;

    AbstractNBodyEngine* m_NBodyEngine;

    // It varies depending on the size of the window.
    float m_pointSizeScale;

    // Display size of particles
    float m_pointSize;

    int m_initHeight;
};
