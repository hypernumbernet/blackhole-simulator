#include "computeshaders.h"

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
        break;
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
