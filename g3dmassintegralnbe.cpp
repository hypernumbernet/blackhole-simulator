#include "g3dmassintegralnbe.h"

G3DMassIntegralNBE::G3DMassIntegralNBE(
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

    quint64 numberOfInteractions = numberOfParticles * (numberOfParticles - 1) / 2;
    m_inversedDistances = new float[numberOfInteractions];

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
    calculateDistances();
}

G3DMassIntegralNBE::~G3DMassIntegralNBE()
{
    delete[] m_mass;
    delete[] m_inversedDistances;
}

bool G3DMassIntegralNBE::calculateDistance(Distance& result, const quint64 a, const quint64 b) const
{
    quint64 ai = a * 3;
    quint64 bi = b * 3;
    float dx = m_coordinates[ai] - m_coordinates[bi]; ++ai; ++bi;
    float dy = m_coordinates[ai] - m_coordinates[bi]; ++ai; ++bi;
    float dz = m_coordinates[ai] - m_coordinates[bi];
    float r = sqrt(dx * dx + dy * dy + dz * dz);
    if (r == 0.0f)
    {
        return false;
    }
    r = 1.0f / r;
    result.invR = r;
    result.unitX = dx * r;
    result.unitY = dy * r;
    result.unitZ = dz * r;
    return true;
}

void G3DMassIntegralNBE::calculateDistances()
{
    quint64 k = 0;
    Distance d;

    for (quint64 i = 0; i < m_numberOfParticles - 1; ++i)
    {
        for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
        {
            if (!calculateDistance(d, i, j))
            {
                continue;
            }
            m_inversedDistances[k] = d.invR;

            ++k;
        }
    }
}

void G3DMassIntegralNBE::calculateTimeProgress() const
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

void G3DMassIntegralNBE::calculateInteraction() const
{
    // Perform integral calculation of universal gravitation.
    // The memory cost is high because the distance data calculated last time is saved and used.
    // However, if the distance variation is large, it may be more accurate to calculate by this method.
    // The celestial scale vibrates violently at float resolution.

    float inv, force;
    Distance d;
    quint64 k = 0, a, b;

    for (quint64 i = 0; i < m_numberOfParticles - 1; ++i)
    {
        for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
        {
            if (!calculateDistance(d, i, j))
                continue;

            inv = m_inversedDistances[k];
            m_inversedDistances[k] = d.invR;
            inv -= d.invR;

            if (inv == 0.0f)
            {
                continue;
            }

            // Time is not taken into account.
            force = inv * GRAVITATIONAL_CONSTANT;

            d.unitX *= force;
            d.unitY *= force;
            d.unitZ *= force;

            a = i * 3;
            b = j * 3;
            m_velocities[a] -= d.unitX * m_mass[j];
            m_velocities[a + 1] -= d.unitY * m_mass[j];
            m_velocities[a + 2] -= d.unitZ * m_mass[j];
            m_velocities[b] += d.unitX * m_mass[i];
            m_velocities[b + 1] += d.unitY * m_mass[i];
            m_velocities[b + 2] += d.unitZ * m_mass[i];

            ++k;
        }
    }
}

void G3DMassIntegralNBE::debug() const
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

void G3DMassIntegralNBE::initParticlesRandam()
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

void G3DMassIntegralNBE::initSunEarth()
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

void G3DMassIntegralNBE::initEarthSun()
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

void G3DMassIntegralNBE::initEarthMoon()
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

void G3DMassIntegralNBE::initSunEarthVenus()
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

void G3DMassIntegralNBE::initTestSamePosition()
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

void G3DMassIntegralNBE::setTimePerFrame(const float time)
{
    m_timePerFrame = time;
    emit m_updateUi->displayTimePerFrame(time);
}
