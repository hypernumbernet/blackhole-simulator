#include "abstractnbodyengine.h"
#include "updateui.h"

template class AbstractNBodyEngine<float>;
template class AbstractNBodyEngine<double>;

template <typename T>
AbstractNBodyEngine<T>::AbstractNBodyEngine(const bhs::SimCondition& sim)
    : m_sim(sim)
    , m_modelScale(1.0)
    , m_gravitationalConstant(T(GRAVITATIONAL_CONSTANT))
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
void AbstractNBodyEngine<T>::setNumberOfParticles()
{
    switch (m_sim.preset)
    {
    case bhs::Preset::RandomCube:
    case bhs::Preset::RandomSphere:
    case bhs::Preset::RandomBall:
        m_numberOfParticles = m_sim.numberOfParticles;
        break;
    case bhs::Preset::Custom:
        m_numberOfParticles = m_sim.custom.particles.size();
        break;
    }
    emit UpdateUi::it().displayNumberOfParticles(m_numberOfParticles);
    int tcount = QThread::idealThreadCount();
    setTimeProgressRanges(tcount);
    setInteractionRanges(tcount);
}

template <typename T>
void AbstractNBodyEngine<T>::setModelScale(const double scale)
{
    m_modelScale = scale / m_scaleInv;
}

template <typename T>
void AbstractNBodyEngine<T>::setModelScaleRatio(const double ratio)
{
    m_modelScale = ratio;
    emit UpdateUi::it().displayModelScale(m_modelScale * m_scaleInv);
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
    m_scaleInv = scale;
    m_modelScale = 1.0;
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
double AbstractNBodyEngine<T>::velocityToAngle(const double v) const
{
    return v * AbstractNBodyEngine<double>::VANGLE / m_scaleInv;
}

template <typename T>
void AbstractNBodyEngine<T>::angleToVelocity(Vector3& a) const
{
    a *= T(m_scaleInv / AbstractNBodyEngine<double>::VANGLE);
}

template <typename T>
void AbstractNBodyEngine<T>::LorentzTransformation(
        QGenericMatrix<4, 4, double>& lt,
        const double vx, const double vy, const double vz) const
{
    double vv = vx * vx + vy * vy + vz * vz;
    if (vv == 0.0)
    {
        lt.setToIdentity();
        return;
    }
    double ci = speedOfLightInv();
    double beta = sqrt(vv) * ci;
    if (beta > 1.0)
    {
        //qDebug() << "beta > 1: " << beta;
        beta = hAbs(fmod(beta - 1.0, 4.0) - 2.0);
    }

    double gamma = 1.0 / sqrt(1.0 - beta * beta);
    if (!std::isfinite(gamma))
    {
        //qDebug() << "gamma INFINITY: " << gamma;
        lt.setToIdentity();
        return;
    }

    double gxc = - gamma * vx * ci;
    double gyc = - gamma * vy * ci;
    double gzc = - gamma * vz * ci;
    double g1 = gamma - 1.0;

    double matrix[] = {
        gamma, gxc, gyc, gzc,
        gxc  , 1.0 + g1 * (vx * vx / vv),       g1 * (vx * vy / vv),       g1 * (vx * vz / vv),
        gyc  ,       g1 * (vx * vy / vv), 1.0 + g1 * (vy * vy / vv),       g1 * (vy * vz / vv),
        gzc  ,       g1 * (vx * vz / vv),       g1 * (vy * vz / vv), 1.0 + g1 * (vz * vz / vv),
    };
    for (int i = 0; i < 16; ++i)
    {
        lt.data()[i] = matrix[i];
    }
}

template <typename T>
void AbstractNBodyEngine<T>::LorentzTransformation(
        QGenericMatrix<4, 4, double>& lt, const Vector3& v) const
{
    LorentzTransformation(lt, v.x(), v.y(), v.z());
}

template <typename T>
const T AbstractNBodyEngine<T>::speedOfLightInv() const
{
    return T(1.0 / (SPEED_OF_LIGHT * m_scaleInv));
}

template <typename T>
double AbstractNBodyEngine<T>::scaleInv() const
{
    return m_scaleInv;
}
