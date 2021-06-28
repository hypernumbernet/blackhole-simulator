#pragma once

#include "g3dmassdiffcore.h"
#include "g3dmassdiffnbe.h"
#include "g3dmassintegralcore.h"
#include "g3dmassintegralnbe.h"
#include "g3svmassdiffcore.h"
#include "g3svmassdiffnbe.h"

#include <math.h>

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class Particles : private QOpenGLFunctions_4_5_Core
{
public:
    explicit Particles(ThreadAdmin*);
    ~Particles();

    bool initialize(int screenHeight);
    void paint(const QMatrix4x4& viewProjection);
    void resize(int height);
    void updateGL();
    void selectNBodyEngine(const bhs::SimCondition&);
    void setModelScale(float);
    void setModelScaleRatio(double);
    void reset(const bhs::SimCondition&);

private:

    //UpdateUi* const m_updateUi;
    ThreadAdmin* const m_threadAdmin;

    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;

    AbstractNBodyEngine* m_NBodyEngine;

    // It varies depending on the size of the window.
    float m_pointSizeScale;

    // Display size of particles
    float m_pointSize;

    int m_initHeight;
};
