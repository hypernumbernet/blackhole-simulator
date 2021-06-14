#pragma once

#include "gravity3dmassdifferentialnbodyengine.h"
#include "updateui.h"

#include <math.h>

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class Particles : public QOpenGLFunctions_3_3_Core
{
public:
    Particles(UpdateUi*, int screenHeight);
    ~Particles();

    bool initialize();
    void paint(QMatrix4x4 viewProjection);
    void resize(int height);
    void updateParticles();
    void selectNBodyEngine();

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

    UpdateUi* m_updateUi;
};
