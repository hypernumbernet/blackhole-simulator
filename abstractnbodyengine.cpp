#include "abstractnbodyengine.h"

#include <QThread>

AbstractNBodyEngine::AbstractNBodyEngine(
        UpdateUi* const updateUi,
        QObject* parent)
    : QObject(parent)
    , m_updateUi(updateUi)
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

    int tcount = QThread::idealThreadCount();

    m_timeProgressRanges.resize(tcount);
    int k = num / tcount;
    for (int i = 0; i < tcount; ++i)
    {
        m_timeProgressRanges[i].start = i * k;
        m_timeProgressRanges[i].numberOfParticles = num;
    }
    for (int i = 0; i < tcount - 1; ++i)
    {
        m_timeProgressRanges[i].end = m_timeProgressRanges[i + 1].start;
    }
    m_timeProgressRanges[tcount - 1].end = num;

    // The length of the side that divides the area of a right-angled isosceles triangle
    // into equal parts is calculated.
    // x = N - sqrt(N^2 - 2y)
    m_interactionRanges.resize(tcount);
    quint64 num_interaction = num * (num - 1) / 2;
    k = num_interaction / tcount;
    int j = num - 1;
    for (int i = 0; i < tcount; ++i)
    {
        m_interactionRanges[i].start = j - (int)floor(sqrt((double)(j * j - 2 * k * i)));
    }
    for (int i = 0; i < tcount - 1; ++i)
    {
        m_interactionRanges[i].end = m_interactionRanges[i + 1].start;
    }
    m_interactionRanges[tcount - 1].end = num;
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

float AbstractNBodyEngine::timePerFrame() const
{
    return m_timePerFrame;
}

int AbstractNBodyEngine::threadCount()
{
    return m_timeProgressRanges.size();
}

void AbstractNBodyEngine::resultReady() const
{
    emit m_updateUi->resultReady();
}
