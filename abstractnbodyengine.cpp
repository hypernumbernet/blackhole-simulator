#include "abstractnbodyengine.h"

AbstractNBodyEngine::AbstractNBodyEngine(UpdateUi* const updateUi, const quint64 numberOfParticles)
    : m_numberOfParticles(numberOfParticles)
    , m_modelScale(1.0f)
    , m_updateUi(updateUi)
{
}

AbstractNBodyEngine::~AbstractNBodyEngine()
{
    delete[] m_coordinates;
    delete[] m_velocities;
}

void AbstractNBodyEngine::setModelScale(const float scale)
{
    m_modelScale = scale;
    emit m_updateUi->setModelScale(QString::number(1.0f / scale));
}
