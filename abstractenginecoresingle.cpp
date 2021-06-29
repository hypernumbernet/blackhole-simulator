#include "abstractenginecoresingle.h"

AbstractEngineCoreSingle::AbstractEngineCoreSingle(AbstractNBodyEngine<float>* const engine, QObject* parent)
    : QObject(parent)
    , m_engine(engine)
    , m_numberOfParticles(engine->numberOfParticle())
    , m_coordinates(engine->coordinates())
    , m_velocities(engine->velocities())
    , m_masses(engine->masses())
    , m_inversedDistances(engine->inversedDistances())
    , m_timePerFrame(engine->timePerFrame())
{

}

void AbstractEngineCoreSingle::resultReady() const
{
    emit UpdateUi::it().resultReady();
}

void AbstractEngineCoreSingle::debug() const
{
    for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
    {
        qDebug() << "C: " << i << m_coordinates[i];
    }
    for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
    {
        qDebug() << "V: " << i << m_velocities[i];
    }
}

bool AbstractEngineCoreSingle::hasRangeTimeProgress(int threadNum) const
{
    auto range = m_engine->timeProgressRanges().at(threadNum);
    return range.end - range.start > 0;
}

bool AbstractEngineCoreSingle::hasRangeInteraction(int threadNum) const
{
    auto range = m_engine->interactionRanges().at(threadNum);
    return range.end - range.start > 0;
}
