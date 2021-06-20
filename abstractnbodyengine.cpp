#include "abstractnbodyengine.h"

AbstractNBodyEngine::AbstractNBodyEngine(UpdateUi* const updateUi)
    : m_updateUi(updateUi)
    , m_modelScale(1.0f)
{
}

AbstractNBodyEngine::~AbstractNBodyEngine()
{
}

void AbstractNBodyEngine::setNumberOfParticles(const quint64 num)
{
    m_numberOfParticles = num;
    emit m_updateUi->displayNumberOfParticles(num);
}

void AbstractNBodyEngine::setModelScale(const float scale)
{
    m_modelScale = scale;
}

void AbstractNBodyEngine::setModelScaleRatio(const float ratio)
{
    m_modelScale = m_scaleCenterValue * ratio;
    emit m_updateUi->displayModelScale(m_modelScale);
}

void AbstractNBodyEngine::changeModelScale(const float scale)
{
    m_modelScale = scale;
    m_scaleCenterValue = scale;
    emit m_updateUi->displayModelScale(scale);
}

quint64 AbstractNBodyEngine::numberOfParticle() const
{
    return m_numberOfParticles;
}

float AbstractNBodyEngine::modelScale() const
{
    return m_modelScale;
}

float* AbstractNBodyEngine::coordinates() const
{
    return m_coordinates;
}

float* AbstractNBodyEngine::masses() const
{
    return m_masses;
}

float* AbstractNBodyEngine::velocities() const
{
    return m_velocities;
}
