#pragma once

#include "g3dmassdiffnbe.h"
#include "g3dmassintegralnbe.h"

#include <math.h>

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class Particles : public QOpenGLFunctions_4_5_Core
{
public:
    Particles(UpdateUi*);
    ~Particles();

    bool initialize(int screenHeight);
    void paint(const QMatrix4x4& viewProjection);
    void resize(int height);
    void updateParticles();
    void selectNBodyEngine(const UpdateUi::SimCondition&);
    void reset(const UpdateUi::SimCondition&);
    void setModelScale(float);
    void setModelScaleRatio(float);

private:
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;

    AbstractNBodyEngine* m_NBodyEngine;

    // It varies depending on the size of the window.
    float m_pointSizeScale;

    // Display size of particles
    float m_pointSize;

    int m_initHeight;
    int m_numberOfParticle;

    UpdateUi* const m_updateUi;
};
