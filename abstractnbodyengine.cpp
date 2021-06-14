#include "abstractnbodyengine.h"

AbstractNBodyEngine::AbstractNBodyEngine(quint64 numberOfParticles)
    : numberOfParticles(numberOfParticles)
{
    coordinates = new float[0];
    velocities = new float[0];
}

AbstractNBodyEngine::~AbstractNBodyEngine()
{
    delete[] coordinates;
    delete[] velocities;
}
