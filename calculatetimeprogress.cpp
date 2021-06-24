#include "calculatetimeprogress.h"

CalculateTimeProgress::CalculateTimeProgress(
        const AbstractNBodyEngine::IntRange range,
        QObject *parent)
    : QObject(parent)
    , m_range(range)
{
    m_coordinates = range.coordinates;
    m_velocities = range.velocities;
    m_masses = range.masses;
    m_timePerFrame = range.timePerFrame;
}
