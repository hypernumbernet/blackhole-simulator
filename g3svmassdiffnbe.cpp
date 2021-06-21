#include "g3svmassdiffnbe.h"

G3SVMassDiffNBE::G3SVMassDiffNBE(
        UpdateUi* const updateUi,
        const bhs::SimCondition& sim)
    : AbstractNBodyEngine(updateUi)
    , Initializer3D(sim, this)
{
    switch (sim.preset) {
    case bhs::Preset::Random:
        setNumberOfParticles(sim.numberOfParticles);
        break;
    case bhs::Preset::SunEarth:
    case bhs::Preset::EarthSun:
    case bhs::Preset::EarthMoon:
    case bhs::Preset::TestSamePosition:
        setNumberOfParticles(2);
        break;
    case bhs::Preset::SunEarthVenus:
        setNumberOfParticles(3);
        break;
    }
    m_masses = new float[m_numberOfParticles];
    m_coordinates = new float[m_numberOfParticles * 3];
    m_velocities = new float[m_numberOfParticles * 3];

    switch (sim.preset) {
    case bhs::Preset::Random:
        initParticlesRandam();
        break;
    case bhs::Preset::SunEarth:
        initSunEarth();
        break;
    case bhs::Preset::EarthSun:
        initEarthSun();
        break;
    case bhs::Preset::EarthMoon:
        initEarthMoon();
        break;
    case bhs::Preset::TestSamePosition:
        initTestSamePosition();
        break;
    case bhs::Preset::SunEarthVenus:
        initSunEarthVenus();
        break;
    }

    setTimePerFrame(sim.timePerFrame);
}

G3SVMassDiffNBE::~G3SVMassDiffNBE()
{
    delete[] m_coordinates;
    delete[] m_velocities;
    delete[] m_masses;
}

void G3SVMassDiffNBE::calculateTimeProgress() const
{
    debug();
    for (quint64 i = 0; i < m_numberOfParticles; ++i)
    {
        quint64 j = i * 3;
        m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
        m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
        m_coordinates[j] += m_velocities[j] * m_timePerFrame;
    }
}

void G3SVMassDiffNBE::calculateInteraction() const
{
    float d1, d2, d3, distance, inv, theta;
    quint64 k = 0, a, b;
    float cinv = (1.0f / SPEED_OF_LIGHT);
    float velangle = M_PI * cinv * 0.5f;

    for (quint64 i = 0; i < m_numberOfParticles - 1; ++i)
    {
        for (quint64 j = i + 1; j < m_numberOfParticles; ++j)
        {
            // Perform differential calculation of universal gravitation.
            a = i * 3;
            b = j * 3;

            d1 = m_coordinates[a] - m_coordinates[b];
            d2 = m_coordinates[a + 1] - m_coordinates[b + 1];
            d3 = m_coordinates[a + 2] - m_coordinates[b + 2];
            distance = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
            if (distance <= 0.0f) {
                continue;
            }
            inv = 1.0f / distance;
            theta = inv * inv * m_timePerFrame * GRAVITATIONAL_CONSTANT;

            d1 *= inv;
            d2 *= inv;
            d3 *= inv;

            auto rota = Quaternion(0.0f, d1, d2, d3);
            rota.MakeRotation(-theta * m_masses[j] * velangle * 0.5f);
            auto rotb = Quaternion(0.0f, d1, d2, d3);
            rotb.MakeRotation(theta * m_masses[i] * velangle * 0.5f);

            auto va = Quaternion<float>::Exp(
                m_velocities[a] * velangle,
                m_velocities[a + 1] * velangle,
                m_velocities[a + 2] * velangle);
            auto vb = Quaternion<float>::Exp(
                m_velocities[b] * velangle,
                m_velocities[b + 1] * velangle,
                m_velocities[b + 2] * velangle);

            auto rotatedA = va.Rot8(rota);
            auto rotatedB = vb.Rot8(rotb);

            auto v3a = rotatedA.LnV3();
            auto v3b = rotatedB.LnV3();
            m_velocities[a] = v3a.x / velangle;
            m_velocities[a + 1] = v3a.y / velangle;
            m_velocities[a + 2] = v3a.z / velangle;
            m_velocities[b] = v3b.x / velangle;
            m_velocities[b + 1] = v3b.y / velangle;
            m_velocities[b + 2] = v3a.z / velangle;

            ++k;
        }
    }
}

void G3SVMassDiffNBE::debug() const
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

void G3SVMassDiffNBE::setTimePerFrame(const float time)
{
    m_timePerFrame = time;
    emit m_updateUi->displayTimePerFrame(time);
}
