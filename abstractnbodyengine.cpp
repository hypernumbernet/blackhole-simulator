#include "abstractnbodyengine.h"

template class AbstractNBodyEngine<float>;
template class AbstractNBodyEngine<double>;

template <typename T>
AbstractNBodyEngine<T>::AbstractNBodyEngine(const bhs::SimCondition& sim)
    : m_sim(sim)
    , m_modelScale(1.0)
    , m_gravitationalConstant(GRAVITATIONAL_CONSTANT)
{
}

template <typename T>
AbstractNBodyEngine<T>::~AbstractNBodyEngine()
{
}

template <typename T>
void AbstractNBodyEngine<T>::setTimeProgressRanges(const int threadCount)
{
    m_timeProgressRanges.resize(threadCount);
    int k = m_numberOfParticles / threadCount;
    for (int i = 0; i < threadCount; ++i)
    {
        m_timeProgressRanges[i].start = i * k;
    }
    for (int i = 0; i < threadCount - 1; ++i)
    {
        m_timeProgressRanges[i].end = m_timeProgressRanges[i + 1].start;
    }
    m_timeProgressRanges[threadCount - 1].end = m_numberOfParticles;
}

// The length of the side that divides the area of a right-angled isosceles triangle
// into equal parts is calculated.
// x = N - sqrt(N^2 - 2y)
template <typename T>
void AbstractNBodyEngine<T>::setInteractionRanges(const int threadCount)
{
    quint64 num = m_numberOfParticles;
    m_interactionRanges.resize(threadCount);
    quint64 num_interaction = num * (num - 1) / 2;
    quint64 k = num_interaction / threadCount;
    int j = num - 1;
    for (int i = 0; i < threadCount; ++i)
    {
        int s = j * j - 2 * k * i;
        if (s < 0)
            m_interactionRanges[i].start = num;
        else
            m_interactionRanges[i].start = j - (int)floor(sqrt((T)s));
    }
    for (int i = 0; i < threadCount - 1; ++i)
    {
        m_interactionRanges[i].end = m_interactionRanges[i + 1].start;
    }
    m_interactionRanges[threadCount - 1].end = num;
}

template <typename T>
void AbstractNBodyEngine<T>::setNumberOfParticles(const quint64 num)
{
    m_numberOfParticles = num;
    emit UpdateUi::it().displayNumberOfParticles(num);
    int tcount = QThread::idealThreadCount();
    setTimeProgressRanges(tcount);
    setInteractionRanges(tcount);
}

template <typename T>
void AbstractNBodyEngine<T>::setModelScale(const double scale)
{
    m_modelScale = scale / m_scaleCenterValue;
}

template <typename T>
void AbstractNBodyEngine<T>::setModelScaleRatio(const double ratio)
{
    m_modelScale = ratio;
    emit UpdateUi::it().displayModelScale(m_modelScale * m_scaleCenterValue);
}

template <typename T>
void AbstractNBodyEngine<T>::setTimePerFrame(const double time)
{
    m_timePerFrame = (T)time;
    emit UpdateUi::it().displayTimePerFrame(time);
}

template <typename T>
void AbstractNBodyEngine<T>::changeModelScale(const double scale)
{
    m_scaleCenterValue = scale;
    m_modelScale = T(1.0);
    emit UpdateUi::it().displayModelScale(scale);
}

template <typename T>
quint64 AbstractNBodyEngine<T>::numberOfParticle() const
{
    return m_numberOfParticles;
}

template <typename T>
double AbstractNBodyEngine<T>::modelScale() const
{
    return m_modelScale;
}

template <typename T>
T* AbstractNBodyEngine<T>::coordinates() const
{
    return m_coordinates;
}

template <typename T>
T* AbstractNBodyEngine<T>::masses() const
{
    return m_masses;
}

template <typename T>
T* AbstractNBodyEngine<T>::distanceInv() const
{
    return m_distanceInv;
}

template <typename T>
T* AbstractNBodyEngine<T>::velocities() const
{
    return m_velocities;
}

template <typename T>
T AbstractNBodyEngine<T>::timePerFrame() const
{
    return m_timePerFrame;
}

template <typename T>
QVector<bhs::IntRange> AbstractNBodyEngine<T>::timeProgressRanges() const
{
    return m_timeProgressRanges;
}

template <typename T>
QVector<bhs::IntRange> AbstractNBodyEngine<T>::interactionRanges() const
{
    return m_interactionRanges;
}

template <typename T>
T* AbstractNBodyEngine<T>::locations() const
{
    return m_locations;
}

template <typename T>
T AbstractNBodyEngine<T>::gravitationalConstant() const
{
    return m_gravitationalConstant;
}

template <typename T>
void AbstractNBodyEngine<T>::setGravitationalConstant(const T g)
{
    m_gravitationalConstant = g;
}

template <typename T>
const bhs::SimCondition& AbstractNBodyEngine<T>::sim() const
{
    return m_sim;
}

template <typename T>
T AbstractNBodyEngine<T>::velocityToAngle(const T v)
{
    return v * T(AbstractNBodyEngine<double>::VANGLE * m_sim.scale);
}

template <typename T>
void AbstractNBodyEngine<T>::angleToVelocity(bhs::Vector3<T>& a)
{
    a *= T(1.0 / (AbstractNBodyEngine<double>::VANGLE * m_sim.scale));
}
