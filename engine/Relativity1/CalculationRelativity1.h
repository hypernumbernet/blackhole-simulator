#pragma once

#include "engine/abstractnbodyengine.h"

using namespace hnn;

/*
The calculation of apsidal precession by general relativity

Formula:

omega = 6 pi G M / (P a (1 - e^2) c^2)
      = 6 pi G M / (P a (b/a)^2 c^2)

omega = (Amount of rotation)(rad/s)
pi = 3.14
G  = 6.67e-11 (Constant of gravitation, m^3 kg^-1 s^-2)
M  = (Solar mass, kg)
P  = (Orbital period, s)
a  = (Semi-major axis, m)
e  = (Eccentricity) = sqrt(1 - (b/a)^2)
b  = (Semi-minor axis, m)
c  = 3e+8 (Speed of light, m)

*/

class CalculationRelativity1
{
public:
    CalculationRelativity1(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : m_engine(engine)
        , m_timeProgresStart(engine->timeProgressRanges().at(threadNumber).start)
        , m_timeProgresEnd(engine->timeProgressRanges().at(threadNumber).end)
        , m_interactionStart(engine->interactionRanges().at(threadNumber).start)
        , m_interactionEnd(engine->interactionRanges().at(threadNumber).end)
        , m_ct(SPEED_OF_LIGHT * engine->scaleInv() * engine->timePerFrame())
        , m_speedOfLightInv(1.0 / (SPEED_OF_LIGHT * engine->scaleInv()))
    {
    }

    static void lorentzTransformation(QGenericMatrix<4, 4, double>& lt, const Vector3& v, const double speedOfLightInv)
    {
        const double vx = v.x(), vy = v.y(), vz = v.z();
        const double vv = vx * vx + vy * vy + vz * vz;
        if (vv == 0.0)
        {
            lt.setToIdentity();
            return;
        }
        double beta = sqrt(vv) * speedOfLightInv;
        if (beta > 1.0)
        {
            //qDebug() << "beta > 1: " << beta;
            beta = hAbs(fmod(beta - 1.0, 4.0) - 2.0);
        }

        const double gamma = 1.0 / sqrt(1.0 - beta * beta);
        if (!std::isfinite(gamma))
        {
            //qDebug() << "gamma INFINITY: " << gamma;
            lt.setToIdentity();
            return;
        }
        const double gc = - gamma * speedOfLightInv;
        const double gxc = vx * gc;
        const double gyc = vy * gc;
        const double gzc = vz * gc;
        const double g1 = gamma - 1.0;

        const double matrix[] = {
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

    void calculateTimeProgress() const
    {
        double* const coordinates = m_engine->coordinates();
        const double* const velocities = m_engine->velocities();

        quint64 i4, i3;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            i4 = i * 4;
            i3 = i * 3;
            double tau = m_ct / velocities[i4];
            //tau *= tau;
            if (!std::isfinite(tau))
            {
                //qDebug() << "tau: " << tau;
                continue;
            }
            coordinates[i3    ] += velocities[i4 + 1] * tau;
            coordinates[i3 + 1] += velocities[i4 + 2] * tau;
            coordinates[i3 + 2] += velocities[i4 + 3] * tau;
        }
    }

    void calculateInteraction() const
    {
        const double* const coordinates = m_engine->coordinates();
        double* const velocities = m_engine->velocities();
        const double* const masses = m_engine->masses();

        const double timePerFrame = m_engine->timePerFrame();
        const double gravitationalConstant = m_engine->gravitationalConstant();
        const double timeG = timePerFrame * gravitationalConstant;
        quint64 numberOfParticles = m_engine->numberOfParticle();
        const double boundaryToInvalidate = AbstractNBodyEngine<double>::BOUNDARY_TO_INVALIDATE;

        double d1, d2, d3, r, inv, theta;
        quint64 i3, j3, i4;
        QGenericMatrix<1, 4, double> speed;
        QGenericMatrix<4, 4, double> acc;
        Vector3 g;

        for (quint64 i = m_timeProgresStart; i < m_timeProgresEnd; ++i)
        {
            i3 = i * 3;
            i4 = i * 4;

            speed(0, 0) = velocities[i4    ];
            speed(1, 0) = velocities[i4 + 1];
            speed(2, 0) = velocities[i4 + 2];
            speed(3, 0) = velocities[i4 + 3];

            for (quint64 j = 0; j < numberOfParticles; ++j)
            {
                if (i == j)
                    continue;

                j3 = j * 3;

                d1 = coordinates[j3    ] - coordinates[i3    ];
                d2 = coordinates[j3 + 1] - coordinates[i3 + 1];
                d3 = coordinates[j3 + 2] - coordinates[i3 + 2];

                r = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                if (r <= boundaryToInvalidate)
                    continue;

                inv = 1.0 / r;
                theta = inv * inv * inv * timeG * masses[j];
                d1 *= theta;
                d2 *= theta;
                d3 *= theta;

                g.set(-d1, -d2, -d3);
                lorentzTransformation(acc, g, m_speedOfLightInv);
                speed = acc * speed;
            }
            velocities[i4    ] = speed(0, 0);
            velocities[i4 + 1] = speed(1, 0);
            velocities[i4 + 2] = speed(2, 0);
            velocities[i4 + 3] = speed(3, 0);
        }
    }

private:
    AbstractNBodyEngine<double>* const m_engine;

    const quint64 m_timeProgresStart;
    const quint64 m_timeProgresEnd;
    const quint64 m_interactionStart;
    const quint64 m_interactionEnd;

    const double m_ct;
    const double m_speedOfLightInv;
};
