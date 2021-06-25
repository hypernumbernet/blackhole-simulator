#include "abstractenginecore.h"

AbstractEngineCore::AbstractEngineCore(AbstractNBodyEngine* const engine, QObject* parent)
    : QObject(parent)
    , m_engine(engine)
    , m_numberOfParticles(engine->numberOfParticle())
    , m_coordinates(engine->coordinates())
    , m_velocities(engine->velocities())
    , m_masses(engine->masses())
    , m_timePerFrame(engine->timePerFrame())
{

}
