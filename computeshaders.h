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
    void bindDouble(AbstractNBodyEngine<double>*);
    void update(quint64 numberOfParticles);
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
