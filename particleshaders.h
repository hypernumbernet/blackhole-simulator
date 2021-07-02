#pragma once

#include "g3dmassdiffnbe.h"
#include "g3dmassdiffcorefloat.h"
#include "g3dmassdiffcoredouble.h"
#include "g3dmassintegralnbe.h"
#include "g3dmassintegralcorefloat.h"
#include "g3dmassintegralcoredouble.h"
#include "g3d4dmassdiffnbe.h"
#include "g3d4dmassdiffcorefloat.h"
#include "g3d4dmassdiffcoredouble.h"
#include "computeshaders.h"

#include <math.h>

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class ParticleShaders : private QOpenGLFunctions_4_5_Core
{
public:
    explicit ParticleShaders(ThreadAdmin*);
    ~ParticleShaders();

    bool initialize(int screenHeight);
    void paint(const QMatrix4x4& viewProjection);
    void resize(int height);
    void updateGL();
    void setNBodyEngine(const bhs::SimCondition&);
    void setModelScale(double);
    void setModelScaleRatio(double);
    void reset(const bhs::SimCondition&);

private:
    quint64 numberOfParticle() const;
    const void* coordinates() const;
    double modelScale() const;
    QOpenGLShaderProgram* program();

    ThreadAdmin* const m_threadAdmin;

    QOpenGLShaderProgram m_programFloat;
    QOpenGLShaderProgram m_programDouble;
    QOpenGLVertexArrayObject m_vao;

    AbstractNBodyEngine<float>* m_NBodyEngineFloat = nullptr;
    AbstractNBodyEngine<double>* m_NBodyEngineDouble = nullptr;

    // It varies depending on the size of the window.
    float m_pointSizeScale;

    // Display size of particles
    float m_pointSize;

    int m_initHeight;
    bhs::Precision m_precision;
    ComputeShaders* const m_computeShaders;
    uint32_t ssbo = 0;
};
