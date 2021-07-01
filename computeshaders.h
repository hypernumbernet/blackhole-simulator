#pragma once

#include "abstractnbodyengine.h"

#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class ComputeShaders : private QOpenGLFunctions_4_5_Core
{
public:
    ComputeShaders();
    ~ComputeShaders();

    bool initialize();
    void bindDouble(AbstractNBodyEngine<double>*);
    void run();
//    GLuint numberOfWorkGroups();

private:
    QOpenGLShaderProgram m_program;

    GLint m_maxComputeWorkSizeX = 0;

    GLint m_maxComputeWorkCountX;
    GLint m_maxComputeWorkCountY;
    GLint m_maxComputeWorkCountZ;
    GLint m_maxConputeWorkInvocations;
    quint64 m_maxComputeWorkCount;

    quint32 m_bufferHandle = 0;
};
