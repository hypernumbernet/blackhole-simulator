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

    if (source == GL_DEBUG_SOURCE_APPLICATION)
        qDebug() << "[" << source_str << "][" << type_str << "][" << id << "][" << severity_str << "] " << msg;
}
#endif

ComputeShaders::ComputeShaders()
{
}

ComputeShaders::~ComputeShaders()
{
}

bool ComputeShaders::addShader(QOpenGLShaderProgram* const pro, const char* file)
{
    if (!pro->addShaderFromSourceFile(QOpenGLShader::Compute, file))
        return false;

    if (!pro->link())
        return false;

    return true;
}

bool ComputeShaders::initialize()
{
    if (!initializeOpenGLFunctions())
        return false;

    if (!addShader(&m_programTimeProgressFloat, ":/shader/timeprogress_f.comp"))
        return false;

    if (!addShader(&m_programInteractionFloat, ":/shader/interaction_f.comp"))
        return false;

    if (!addShader(&m_programTimeProgressDouble, ":/shader/timeprogress_d.comp"))
        return false;

    if (!addShader(&m_programInteractionDouble, ":/shader/interaction_d.comp"))
        return false;

    if (!addShader(&m_programTimeProgress3D4DFloat, ":/shader/timeprogress3D4D_f.comp"))
        return false;

    if (!addShader(&m_programInteraction3D4DFloat, ":/shader/interaction3D4D_f.comp"))
        return false;

    if (!addShader(&m_programTimeProgress3D4DDouble, ":/shader/timeprogress3D4D_d.comp"))
        return false;

    if (!addShader(&m_programInteraction3D4DDouble, ":/shader/interaction3D4D_d.comp"))
        return false;

#ifdef QT_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

    return true;
}

void ComputeShaders::bind(const AbstractNBodyEngine<float>* const engine)
{
    m_numberOfWorkGroups = (engine->numberOfParticle() + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE;
    switch (engine->m_sim.engine)
    {
    case bhs::Engine::G3DEuler:
        m_programTimeProgressUsing = &m_programTimeProgressFloat;
        m_programInteractionUsing = &m_programInteractionFloat;
        break;
    case bhs::Engine::G3D4DEuler:
        m_programTimeProgressUsing = &m_programTimeProgress3D4DFloat;
        m_programInteractionUsing = &m_programInteraction3D4DFloat;
        break;
    }
}

void ComputeShaders::bind(const AbstractNBodyEngine<double>* const engine)
{
    m_numberOfWorkGroups = (engine->numberOfParticle() + WORK_GROUP_SIZE - 1) / WORK_GROUP_SIZE;
    switch (engine->m_sim.engine)
    {
    case bhs::Engine::G3DEuler:
        m_programTimeProgressUsing = &m_programTimeProgressDouble;
        m_programInteractionUsing = &m_programInteractionDouble;
    case bhs::Engine::G3D4DEuler:
        m_programTimeProgressUsing = &m_programTimeProgress3D4DDouble;
        m_programInteractionUsing = &m_programInteraction3D4DDouble;
        break;
    }
}

void ComputeShaders::update()
{
    m_programTimeProgressUsing->bind();
    glDispatchCompute(m_numberOfWorkGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    m_programInteractionUsing->bind();
    glDispatchCompute(m_numberOfWorkGroups, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
