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
//    GLuint numberOfWorkGroups();

private:
    QOpenGLShaderProgram m_programTimeProgress;
    QOpenGLShaderProgram m_programInteraction;
    GLuint m_numberOfWorkGroups = 0;

//    GLint m_maxComputeWorkSizeX = 0;
//    GLint m_maxComputeWorkCountX;
//    GLint m_maxComputeWorkCountY;
//    GLint m_maxComputeWorkCountZ;
//    GLint m_maxConputeWorkInvocations;
//    quint64 m_maxComputeWorkCount;
};
