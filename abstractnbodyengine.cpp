#include "abstractnbodyengine.h"

AbstractNBodyEngine::AbstractNBodyEngine(UpdateUi* updateUi, quint64 numberOfParticles)
    : numberOfParticles(numberOfParticles)
    , m_modelScale(1.0f)
    , m_updateUi(updateUi)
{
}

AbstractNBodyEngine::~AbstractNBodyEngine()
{
    delete[] coordinates;
    delete[] velocities;
}

void AbstractNBodyEngine::setModelScale(float scale)
{
    m_modelScale = scale;
    emit m_updateUi->setModelScale(QString::number(1.0f / scale));
}
