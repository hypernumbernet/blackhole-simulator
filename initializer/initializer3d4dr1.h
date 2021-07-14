#pragma once

#include "initializer3d.h"
#include "hnn/quaternion.h"

using namespace hnn;

template <typename T>
class Initializer3D4DR1 : public AbstractInitializer
{
public:
    Initializer3D4DR1(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
        , m_3d(sim, engine)
    {
    }

protected:
    void initRandamCube() override;
    void initRandamSphere(double) override;
    void initSunEarth() override;
    void initEarthSun() override;
    void initEarthMoon() override;
    void initSunEarthVenus() override;
    void initTestSamePosition() override;

private:

    inline QGenericMatrix<1, 4, T> fromVector3(Vector3<T> v3) const
    {
        auto lt = m_engine->LorentzTransformation(v3);
        QGenericMatrix<1, 4, T> speed;
        speed(0, 0) = T(1) / m_engine->timePerFrame();
        speed(0, 1) = T(0);
        speed(0, 2) = T(0);
        speed(0, 3) = T(0);
        return lt * speed;
    }

    inline void fromInitializer3D()
    {
        const quint64 num = m_engine->numberOfParticle();
        T* const velocities = m_engine->velocities();

        T* vels = new T[num * 4];

        for (quint64 i = 0; i < num; ++i)
        {
            quint64 i3 = i * 3;
            quint64 i4 = i * 4;

            auto vel4 = fromVector3({velocities, i3});
            bhs::embedMatrix1x4ToArray<T>(vel4, vels, i4);
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
