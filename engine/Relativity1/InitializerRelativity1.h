#pragma once

#include "engine/3d/initializer3d.h"
#include "CalculationRelativity1.h"

using namespace hnn;

template <typename T>
class InitializerRelativity1 : protected AbstractInitializer
{
public:
    InitializerRelativity1(const bhs::SimCondition& sim, AbstractNBodyEngine<T>* const engine)
        : AbstractInitializer(sim)
        , m_engine(engine)
        , m_3d(sim, engine)
    {
    }

private:
    void initRandomCube() override;
    void initRandomSphere(double) override;
    void initCustom() override;

    QGenericMatrix<1, 4, double> fromVector3(const Vector3& v3) const
    {
        QGenericMatrix<4, 4, double> lt;
        CalculationRelativity1::lorentzTransformation(lt, -v3, m_engine->speedOfLightInv());
        QGenericMatrix<1, 4, double> speed;
        speed(0, 0) = SPEED_OF_LIGHT * m_engine->scaleInv() * m_engine->timePerFrame();
        speed(1, 0) = 0.0;
        speed(2, 0) = 0.0;
        speed(3, 0) = 0.0;
        return lt * speed;
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
