#include "computeshaders.h"

#include <QDebug>
#include <iterator>

#ifdef QT_DEBUG
void APIENTRY gl_debug_callback(
        GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei/* length*/, const GLchar* msg, const void* /*data*/)
{
    QString source_str;
    QString type_str;
    QString severity_str;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        source_str = "SOURCE_API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        source_str = "SOURCE_WINDOW_SYSTEM";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        source_str = "SOURCE_SHADER_COMPILER";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        source_str = "SOURCE_THIRD_PARTY";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        source_str = "SOURCE_APPLICATION";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        source_str = "SOURCE_OTHER";
        break;
    default:
        source_str = "?";
        break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        type_str = "TYPE_ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        type_str = "TYPE_DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        type_str = "TYPE_UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        type_str = "TYPE_PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        type_str = "TYPE_PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        type_str = "TYPE_OTHER";
        break;
    case GL_DEBUG_TYPE_MARKER:
        type_str = "TYPE_MARKER";
        break;
    default:
        type_str = "?";
        break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        severity_str = "SEVERITY_HIGH";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        severity_str = "SEVERITY_MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        severity_str = "SEVERITY_LOW";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        severity_str = "SEVERITY_NOTIFICATION";
        break;
    default:
        severity_str = "?";
        break;
    }
    qDebug() << "[" << source_str << "][" << type_str << "][" << id << "][" << severity_str << "] " << msg;
}
#endif

ComputeShaders::ComputeShaders()
{
}

ComputeShaders::~ComputeShaders()
{
}

bool ComputeShaders::initialize()
{
    if (!initializeOpenGLFunctions()) {
        return false;
    }
    if (!m_programTimeProgress.addShaderFromSourceFile(QOpenGLShader::Compute, ":/shader/timeprogress_d.comp")) {
        return false;
    }
    if (!m_programTimeProgress.link()) {
        return false;
    }
    if (!m_programInteraction.addShaderFromSourceFile(QOpenGLShader::Compute, ":/shader/interaction_d.comp")) {
        return false;
    }
    if (!m_programInteraction.link()) {
        return false;
    }

//    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &m_maxComputeWorkSizeX);
//    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &m_maxComputeWorkCountX);
//    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &m_maxComputeWorkCountY);
//    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &m_maxComputeWorkCountZ);
//    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &m_maxConputeWorkInvocations);
//    m_maxComputeWorkCount = m_maxComputeWorkCountX * m_maxComputeWorkCountY * m_maxComputeWorkCountZ;

#ifdef QT_DEBUG
//    glEnable(GL_DEBUG_OUTPUT);
//    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//    glDebugMessageCallback(gl_debug_callback, nullptr);
//    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

    return true;
}

void ComputeShaders::bind(AbstractNBodyEngine<float>* engine)
{
    m_numberOfWorkGroups = (engine->numberOfParticle() + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE;
}

void ComputeShaders::bind(AbstractNBodyEngine<double>* engine)
{
    m_numberOfWorkGroups = (engine->numberOfParticle() + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE;
}

void ComputeShaders::update()
{
    m_programTimeProgress.bind();
    glDispatchCompute(m_numberOfWorkGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    m_programInteraction.bind();
    glDispatchCompute(m_numberOfWorkGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

//GLuint ComputeShaders::numberOfWorkGroups()
//{
//    return
//}
