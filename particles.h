#pragma once

#include "gravity3dmassdifferentialnbodyengine.h"
#include "updateui.h"

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
    void selectNBodyEngine();
    void reset();

private:
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject m_vao;

    AbstractNBodyEngine* m_NBodyEngine;

    // Windowの大きさで変動する
    float pointSizeScale;

    // 粒子の表示上の大きさ
    float pointSize;

    int initHeight;
    int numberOfParticle;

    UpdateUi* const m_updateUi;
};
