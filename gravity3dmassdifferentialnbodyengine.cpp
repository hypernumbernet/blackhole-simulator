#include "gravity3dmassdifferentialnbodyengine.h"

Gravity3DMassDifferentialNBodyEngine::Gravity3DMassDifferentialNBodyEngine(
        UpdateUi* const updateUi,
        const quint64 numberOfParticles,
        const float timePerFrame,
        const Preset presetNumber)
    : AbstractNBodyEngine(updateUi)
{
    switch (presetNumber) {
    case Preset::Random:
        setNumberOfParticles(numberOfParticles);
        break;
    case Preset::SunEarth:
    case Preset::EarthSun:
        setNumberOfParticles(2);
        break;
    }

    //inversedDistances = new float[0];

    m_mass = new float[m_numberOfParticles];
    m_coordinates = new float[m_numberOfParticles * 3];
    m_velocities = new float[m_numberOfParticles * 3];

    //numberOfInteractions = numberOfParticles * (numberOfParticles - 1) / 2;
    //inversedDistances = new float[numberOfInteractions];

    switch (presetNumber) {
    case Preset::Random:
        initParticlesRandam();
        break;
    case Preset::SunEarth:
        initSunEarth();
        break;
    case Preset::EarthSun:
        initEarthSun();
        break;
    }

    setTimePerFrame(timePerFrame);
}

Gravity3DMassDifferentialNBodyEngine::~Gravity3DMassDifferentialNBodyEngine()
{
    delete[] m_mass;
    //delete[] inversedDistances;
}

float Gravity3DMassDifferentialNBodyEngine::calculateDistance(const quint64 a, const quint64 b) const
{
    quint64 ai = a * 3;
    quint64 bi = b * 3;
    float d1 = m_coordinates[ai] - m_coordinates[bi]; ++ai; ++bi;
    float d2 = m_coordinates[ai] - m_coordinates[bi]; ++ai; ++bi;
    float d3 = m_coordinates[ai] - m_coordinates[bi];
    return sqrt(d1 * d1 + d2 * d2 + d3 * d3);
}

//void Gravity3DMassDifferentialNBodyEngine::calculateDistances()
//{
//quint64 k = 0;
//for (quint64 i = 0; i < numberOfParticles - 1; ++i)
//{
//    for (quint64 j = i + 1; j < numberOfParticles; ++j)
//    {
//        inversedDistances[k] = 1.0f / getDistance(i, j);
//        ++k;
//    }
//}
//}

void Gravity3DMassDifferentialNBodyEngine::timeProgress() const
{
    //debug();
    for (quint64 i = 0; i < m_numberOfParticles; ++i)
    {
        quint64 j = i * 3;
        m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
        m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
        m_coordinates[j] += m_velocities[j] * m_timePerFrame;
    }
}

void Gravity3DMassDifferentialNBodyEngine::calculateInteraction() const
{
    float theta;
    quint64 k = 0;
    for (quint64 i = 0; i < m_numberOfParticles - 1; ++i)
    {
        for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
        {
            // 積分計算
            // 前回計算した距離データを使用するのでメモリコストが高い。
            // しかし、距離変動が大きい場合はこちらで計算した方が正確か？
            //theta = inversedDistances[k];
            //inversedDistances[k] = 1.0f / getDistance(i, j);
            //theta -= inversedDistances[k];

            // 微分計算
            theta = 1.0f / calculateDistance(i, j);
            theta *= theta * m_timePerFrame;

            if (theta == theta)
            {
                theta *= GRAVITATIONAL_CONSTANT;

                quint64 a = i * 3;
                quint64 b = j * 3;
                float d1 = m_coordinates[a] - m_coordinates[b];
                float d2 = m_coordinates[a + 1] - m_coordinates[b + 1];
                float d3 = m_coordinates[a + 2] - m_coordinates[b + 2];
                float norm = 1.0f / sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                d1 *= theta * norm;
                d2 *= theta * norm;
                d3 *= theta * norm;

                m_velocities[a] -= d1 * m_mass[j];
                m_velocities[a + 1] -= d2 * m_mass[j];
                m_velocities[a + 2] -= d3 * m_mass[j];
                m_velocities[b] += d1 * m_mass[i];
                m_velocities[b + 1] += d2 * m_mass[i];
                m_velocities[b + 2] += d3 * m_mass[i];

            } else {
                qDebug() << "[warning] invalid number detected";
            }

            ++k;
        }
    }
}

void Gravity3DMassDifferentialNBodyEngine::debug() const
{
    for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
    {
        qDebug() << "xyz:" << i << m_coordinates[i];
    }
    for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
    {
        qDebug() << "v:" << i << m_velocities[i];
    }
}

quint64 Gravity3DMassDifferentialNBodyEngine::numberOfParticle() const
{
    return m_numberOfParticles;
}

float Gravity3DMassDifferentialNBodyEngine::modelScale() const
{
    return m_modelScale;
}

float* Gravity3DMassDifferentialNBodyEngine::coordinates() const
{
    return m_coordinates;
}

void Gravity3DMassDifferentialNBodyEngine::initParticlesRandam()
{
    setModelScale(1.0e-3f);
    for (quint64 i = 0; i < m_numberOfParticles; ++i)
    {
        m_mass[i] = randf() * 2.0e+2f - 1.0e+2f;
        //mass[i] = 2.0e+2f;
    }
    for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
    {
        m_coordinates[i] = randf() * 2.0e+3f - 1.0e+3f;
    }
    for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
    {
        //velocities[i] = randf() * 2.0e-7f - 1.0e-7f;
        m_velocities[i] = 0.0f;
    }
    //calculateDistances();
}

void Gravity3DMassDifferentialNBodyEngine::initSunEarth()
{
    setModelScale(1.0e-11f);
    m_mass[0] = 1.9891e+30f;
    m_coordinates[0] = m_coordinates[1] = m_coordinates[2] = 0.0f;
    m_velocities[0] = m_velocities[1] = m_velocities[2] = 0.0f;

    m_mass[1] = 5.972e+24f;
    m_coordinates[3] = 1.495978e+11f;
    m_coordinates[4] = 0.0f;
    m_coordinates[5] = 0.0f;
    m_velocities[3] = 0.0f;
    m_velocities[4] = 29780.0f;
    m_velocities[5] = 0.0f;
}

void Gravity3DMassDifferentialNBodyEngine::initEarthSun()
{
    setModelScale(1.0e-11f);
    m_mass[1] = 1.9891e+30f;
    m_coordinates[3] = m_coordinates[4] = m_coordinates[5] = 0.0f;
    m_velocities[3] = m_velocities[4] = m_velocities[5] = 0.0f;

    m_mass[0] = 5.972e+24f;
    m_coordinates[0] = 1.495978e+11f;
    m_coordinates[1] = 0.0f;
    m_coordinates[2] = 0.0f;
    m_velocities[0] = 0.0f;
    m_velocities[1] = 29780.0f;
    m_velocities[2] = 0.0f;
}

void Gravity3DMassDifferentialNBodyEngine::setTimePerFrame(const float time)
{
    m_timePerFrame = time;
    emit m_updateUi->showTimePerFrame(QString::number(time));
}
