#pragma once

#include "engine/3d/initializer3d.h"

using namespace hnn;

template <typename T>
class InitializerRelativity2 : protected AbstractInitializer
{
public:
    InitializerRelativity2(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
        , m_3d(sim, engine)
    {
    }

private:
    void initRandomCube() override;
    void initRandomSphere(double) override;
    void initCustom() override;

    Spacetime fromVector3(const Vector3& v) const
    {
        Spacetime st(SPEED_OF_LIGHT * m_engine->scaleInv() * m_engine->timePerFrame(), 0., 0., 0.);
        st.lorentzTransformation(v, m_engine->speedOfLightInv());
        return st;
    }

    void fromInitializer3D()
    {
        const quint64 num = m_engine->numberOfParticle();
        T* const velocities = m_engine->velocities();

        T* const vels = new T[num * 4];

        for (quint64 i = 0; i < num; ++i)
        {
            quint64 i3 = i * 3;
            quint64 i4 = i * 4;
            const Vector3 v((double)velocities[i3], (double)velocities[i3 + 1], (double)velocities[i3 + 2]);
            auto vel4 = fromVector3(v);
            vels[i4    ] = (T)vel4.w();
            vels[i4 + 1] = (T)vel4.x();
            vels[i4 + 2] = (T)vel4.y();
            vels[i4 + 3] = (T)vel4.z();
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
