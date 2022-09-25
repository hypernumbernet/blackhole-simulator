#pragma once

#include "hnn/quaternion.h"
#include "engine/3D/Initializer3D.h"

using namespace hnn;

template <typename T>
class InitializerUniverse1 : protected AbstractInitializer
{
public:
    static constexpr double ZOOM = 32.;

    InitializerUniverse1(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
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

        T* const velocities = m_engine->velocities();
        T* vels = new T[num * 4];
        Vector3 vv;
        Quaternion vq;

        for (quint64 i = 0; i < num; ++i)
        {
            quint64 i3 = i * 3;
            quint64 i4 = i * 4;
            vv.set((double)velocities[i3], (double)velocities[i3 + 1], (double)velocities[i3 + 2]);
            vq.set(Quaternion::exp(vv * coordToAngle));
            bhs::embedQuaternionToArray(vq, vels, i4);
        }
        for (quint64 i = 0; i < num * 4; ++i)
        {
            velocities[i] = vels[i];
        }

        delete[] vels;
    }

    AbstractNBodyEngine<T>* const m_engine;

    Initializer3D<T> m_3d;
};
