#pragma once

#include "hnn/quaternion.h"
#include "engine/3D/Initializer3D.h"

using namespace hnn;

template <typename T>
class InitializerUniverse2 : protected AbstractInitializer
{
public:
    static constexpr double ZOOM = 32.;

    InitializerUniverse2(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
        , m_3d(sim, engine)
    {
    }

private:
    void initRandomCube() override;
    void initRandomSphere(double) override;
    void initCustom() override;

    void fromInitializer3D()
    {
        const quint64 num = m_engine->numberOfParticle();
        const T* const coordinates = m_engine->coordinates();
        T* const locations = m_engine->locations();
        const double coordToAngle = PI / ZOOM;

        for (quint64 i = 0; i < num; ++i)
        {
            quint64 i3 = i * 3;
            quint64 i4 = i * 4;

            Vector3 coord((double)coordinates[i3], (double)coordinates[i3 + 1], (double)coordinates[i3 + 2]);
            coord *= coordToAngle;
            Quaternion qc = Quaternion::exp(coord);
            bhs::embedQuaternionToArray<T>(qc, locations, i4);
        }
    }

    AbstractNBodyEngine<T>* const m_engine;

    Initializer3D<T> m_3d;
};
