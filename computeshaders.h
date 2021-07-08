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
    void bind(const AbstractNBodyEngine<float>*);
    void bind(const AbstractNBodyEngine<double>*);
    void update();

private:
    bool addShader(QOpenGLShaderProgram*, const char*);
    void useTimeProgressProgram();
    void useInteractionProgram();

    QOpenGLShaderProgram m_programTimeProgressFloat;
    QOpenGLShaderProgram m_programInteractionFloat;

    QOpenGLShaderProgram m_programTimeProgressDouble;
    QOpenGLShaderProgram m_programInteractionDouble;

    QOpenGLShaderProgram m_programTimeProgress3D4DFloat;
    QOpenGLShaderProgram m_programInteraction3D4DFloat;

    QOpenGLShaderProgram m_programTimeProgress3D4DDouble;
    QOpenGLShaderProgram m_programInteraction3D4DDouble;

    QOpenGLShaderProgram* m_programTimeProgressUsing = nullptr;
    QOpenGLShaderProgram* m_programInteractionUsing = nullptr;

    GLuint m_numberOfWorkGroups = 0;
};
