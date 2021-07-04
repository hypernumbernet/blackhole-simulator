#pragma once

#include "abstractnbodyengine.h"

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class ComputeShaders : private QOpenGLFunctions_4_5_Core
{
    static constexpr GLuint WORK_GROUP_SIZE = 128;

public:
    ComputeShaders();
    ~ComputeShaders();

    bool initialize();
    void bind(AbstractNBodyEngine<float>*);
    void bind(AbstractNBodyEngine<double>*);
    void update();

private:
    bool addShader(QOpenGLShaderProgram*, const char*);

    QOpenGLShaderProgram m_programTimeProgressFloat;
    QOpenGLShaderProgram m_programInteractionFloat;
    QOpenGLShaderProgram m_programTimeProgressDouble;
    QOpenGLShaderProgram m_programInteractionDouble;
    GLuint m_numberOfWorkGroups = 0;
    bhs::Precision m_precision;
};
