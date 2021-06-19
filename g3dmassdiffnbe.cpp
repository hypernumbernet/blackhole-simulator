#include "g3dmassdiffnbe.h"

G3DMassDiffNBE::G3DMassDiffNBE(
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
    case Preset::EarthMoon:
    case Preset::TestSamePosition:
        setNumberOfParticles(2);
        break;
    case Preset::SunEarthVenus:
        setNumberOfParticles(3);
        break;
    }
    m_mass = new float[m_numberOfParticles];
    m_coordinates = new float[m_numberOfParticles * 3];
    m_velocities = new float[m_numberOfParticles * 3];

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
    case Preset::EarthMoon:
        initEarthMoon();
        break;
    case Preset::TestSamePosition:
        initTestSamePosition();
        break;
    case Preset::SunEarthVenus:
        initSunEarthVenus();
        break;
    }

    setTimePerFrame(timePerFrame);
}

G3DMassDiffNBE::~G3DMassDiffNBE()
{
    delete[] m_mass;
}

void G3DMassDiffNBE::calculateTimeProgress() const
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

void G3DMassDiffNBE::calculateInteraction() const
{
    float d1, d2, d3, distance, inv, force;
    quint64 k = 0, a, b;
    for (quint64 i = 0; i < m_numberOfParticles - 1; ++i)
    {
        for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
        {
            // 微分計算
            a = i * 3;
            b = j * 3;
            d1 = m_coordinates[a] - m_coordinates[b];
            d2 = m_coordinates[a + 1] - m_coordinates[b + 1];
            d3 = m_coordinates[a + 2] - m_coordinates[b + 2];
            distance = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
            if (distance == 0.0f) {
                continue;
            }
            inv = 1.0f / distance;
            force = inv * inv * m_timePerFrame * GRAVITATIONAL_CONSTANT;

            d1 *= inv * force;
            d2 *= inv * force;
            d3 *= inv * force;

            m_velocities[a] -= d1 * m_mass[j];
            m_velocities[a + 1] -= d2 * m_mass[j];
            m_velocities[a + 2] -= d3 * m_mass[j];
            m_velocities[b] += d1 * m_mass[i];
            m_velocities[b + 1] += d2 * m_mass[i];
            m_velocities[b + 2] += d3 * m_mass[i];

            ++k;
        }
    }
}

void G3DMassDiffNBE::debug() const
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

void G3DMassDiffNBE::initParticlesRandam()
{
    changeModelScale(1.0e-3f);
    for (quint64 i = 0; i < m_numberOfParticles; ++i)
    {
        m_mass[i] = randf() * 2.0e+2f - 1.0e+2f;
        //m_mass[i] = 2.0e+2f;
    }
    for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
    {
        m_coordinates[i] = randf() * 2.0e+3f - 1.0e+3f;
    }
    for (quint64 i = 0; i < m_numberOfParticles * 3; ++i)
    {
        m_velocities[i] = randf() * 2.0e-7f - 1.0e-7f;
        //m_velocities[i] = 0.0f;
    }
    //calculateDistances();

    m_mass[0] = 2.0e+5f;
//    m_coordinates[0] = 0.0f;
//    m_coordinates[1] = 0.0f;
//    m_coordinates[2] = 0.0f;

}

void G3DMassDiffNBE::initSunEarth()
{
    changeModelScale(1.0e-11f);
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

void G3DMassDiffNBE::initEarthSun()
{
    changeModelScale(1.0e-11f);

    m_mass[0] = 5.972e+24f;
    m_coordinates[0] = 0.0f;
    m_coordinates[1] = 0.0f;
    m_coordinates[2] = 0.0f;
    m_velocities[0] = 0.0f;
    m_velocities[1] = 29780.0f;
    m_velocities[2] = 0.0f;

    m_mass[1] = 1.9891e+30f;
    m_coordinates[3] = 1.495978e+11f;
    m_coordinates[4] = 0.0f;
    m_coordinates[5] = 0.0f;
    m_velocities[3] = 0.0f;
    m_velocities[4] = 0.0f;
    m_velocities[5] = 0.0f;
}

void G3DMassDiffNBE::initEarthMoon()
{
    changeModelScale(0.25e-8f);

    m_mass[0] = 5.972e+24f;
    m_coordinates[0] = 0.0f;
    m_coordinates[1] = 0.0f;
    m_coordinates[2] = 0.0f;
    m_velocities[0] = 0.0f;
    m_velocities[1] = 0.0f;
    m_velocities[2] = 0.0f;

    m_mass[1] = 7.347673e+22f;
    m_coordinates[3] = 3.844e+8f;
    m_coordinates[4] = 0.0f;
    m_coordinates[5] = 0.0f;
    m_velocities[3] = 0.0f;
    m_velocities[4] = 1022.0f;
    m_velocities[5] = 0.0f;
}

void G3DMassDiffNBE::initSunEarthVenus()
{
    changeModelScale(1.0e-11f);

    m_mass[0] = 1.9891e+30f;
    m_coordinates[0] = 0.0f;
    m_coordinates[1] = 0.0f;
    m_coordinates[2] = 0.0f;
    m_velocities[0] = 0.0f;
    m_velocities[1] = 0.0f;
    m_velocities[2] = 0.0f;

    m_mass[1] = 5.972e+24f;
    m_coordinates[3] = 1.495978e+11f;
    m_coordinates[4] = 0.0f;
    m_coordinates[5] = 0.0f;
    m_velocities[3] = 0.0f;
    m_velocities[4] = 0.0f;
    m_velocities[5] = 29780.0f;

    m_mass[2] = 4.869e+24f;
    m_coordinates[6] = 1.0820893e+11f;
    m_coordinates[7] = 0.0f;
    m_coordinates[8] = 0.0f;
    m_velocities[6] = 0.0f;
    m_velocities[7] = 0.0f;
    m_velocities[8] = 35021.4f;
}

void G3DMassDiffNBE::initTestSamePosition()
{
    changeModelScale(1.0f);

    m_mass[0] = 1.0e+10f;
    m_coordinates[0] = 1.0f;
    m_coordinates[1] = 2.0f;
    m_coordinates[2] = 3.0f;
    m_velocities[0] = 0.0f;
    m_velocities[1] = 0.0f;
    m_velocities[2] = 0.0f;

    m_mass[1] = 2.0e+10f;
    m_coordinates[3] = 1.0f;
    m_coordinates[4] = 2.0f;
    m_coordinates[5] = 3.0f;
    m_velocities[3] = 0.0f;
    m_velocities[4] = 0.0f;
    m_velocities[5] = 0.0f;
}

void G3DMassDiffNBE::setTimePerFrame(const float time)
{
    m_timePerFrame = time;
    emit m_updateUi->displayTimePerFrame(time);
}

QString G3DMassDiffNBE::name() const
{
    return QString("Gravity3DMassDifferential");
}
