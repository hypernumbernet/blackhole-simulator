#include "abstractnbodyengine.h"

AbstractNBodyEngine::AbstractNBodyEngine(quint64 numberOfParticles)
    : numberOfParticles(numberOfParticles)
    , modelScale(1.0f)
{
}

AbstractNBodyEngine::~AbstractNBodyEngine()
{
    delete[] coordinates;
    delete[] velocities;
}
