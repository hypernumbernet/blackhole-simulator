#include "gravity3dmassdifferentialnbodyengine.h"

Gravity3DMassDifferentialNBodyEngine::Gravity3DMassDifferentialNBodyEngine(quint64 numberOfParticles, float timePerFrame)
    : AbstractNBodyEngine(numberOfParticles)
    , timePerFrame(timePerFrame)
{
    mass = new float[0];
    //inversedDistances = new float[0];
}

Gravity3DMassDifferentialNBodyEngine::~Gravity3DMassDifferentialNBodyEngine()
{
    delete[] mass;
    //delete[] inversedDistances;
}

void Gravity3DMassDifferentialNBodyEngine::newParticles()
{
    delete[] mass;
    delete[] coordinates;
    delete[] velocities;
    //delete[] inversedDistances;

    mass = new float[numberOfParticles];
    coordinates = new float[numberOfParticles * 3];
    velocities = new float[numberOfParticles * 3];

    //numberOfInteractions = numberOfParticles * (numberOfParticles - 1) / 2;
    //inversedDistances = new float[numberOfInteractions];
}

float Gravity3DMassDifferentialNBodyEngine::getDistance(quint64 a, quint64 b)
{
    quint64 ai = a * 3;
    quint64 bi = b * 3;
    float d1 = coordinates[ai] - coordinates[bi]; ++ai; ++bi;
    float d2 = coordinates[ai] - coordinates[bi]; ++ai; ++bi;
    float d3 = coordinates[ai] - coordinates[bi];
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

void Gravity3DMassDifferentialNBodyEngine::timeProgress()
{
    //debug();
    for (quint64 i = 0; i < numberOfParticles; ++i)
    {
        quint64 j = i * 3;
        coordinates[j] += velocities[j] * timePerFrame; ++j;
        coordinates[j] += velocities[j] * timePerFrame; ++j;
        coordinates[j] += velocities[j] * timePerFrame;
    }
}

void Gravity3DMassDifferentialNBodyEngine::calculateInteraction()
{
    float theta;
    quint64 k = 0;
    for (quint64 i = 0; i < numberOfParticles - 1; ++i)
    {
        for (quint64 j = i + 1; j < numberOfParticles; ++j)
        {
            // 積分計算
            // 前回計算した距離データを使用するのでメモリコストが高い。
            // しかし、距離変動が大きい場合はこちらで計算した方が正確か？
            //theta = inversedDistances[k];
            //inversedDistances[k] = 1.0f / getDistance(i, j);
            //theta -= inversedDistances[k];

            // 微分計算
            theta = 1.0f / getDistance(i, j);
            theta *= theta * timePerFrame;

            if (theta == theta)
            {
                theta *= GRAVITATIONAL_CONSTANT;

                quint64 a = i * 3;
                quint64 b = j * 3;
                float d1 = coordinates[a] - coordinates[b];
                float d2 = coordinates[a + 1] - coordinates[b + 1];
                float d3 = coordinates[a + 2] - coordinates[b + 2];
                float norm = sqrt(d1 * d1 + d2 * d2 + d3 * d3);
                d1 *= theta / norm;
                d2 *= theta / norm;
                d3 *= theta / norm;

                velocities[a] -= d1 * mass[j];
                velocities[a + 1] -= d2 * mass[j];
                velocities[a + 2] -= d3 * mass[j];
                velocities[b] += d1 * mass[i];
                velocities[b + 1] += d2 * mass[i];
                velocities[b + 2] += d3 * mass[i];

            } else {
                qDebug() << "[warning] invalid number detected";
            }

            ++k;
        }
    }
}

void Gravity3DMassDifferentialNBodyEngine::debug()
{
    for (quint64 i = 0; i < numberOfParticles * 3; ++i)
    {
        qDebug() << "xyz:" << i << coordinates[i];
    }
    for (quint64 i = 0; i < numberOfParticles * 3; ++i)
    {
        qDebug() << "v:" << i << velocities[i];
    }
}

quint64 Gravity3DMassDifferentialNBodyEngine::getNumberOfParticle()
{
    return numberOfParticles;
}

void Gravity3DMassDifferentialNBodyEngine::initialize(int presetNumber)
{
    switch (presetNumber) {
    case 0:
        initParticlesRandam();
        break;
    case 1:
        initSunEarth();
        break;
    default:
        initParticlesRandam();
    }
}

float* Gravity3DMassDifferentialNBodyEngine::getCoordinates()
{
    return coordinates;
}

void Gravity3DMassDifferentialNBodyEngine::initParticlesRandam()
{
    for (quint64 i = 0; i < numberOfParticles; ++i)
    {
        mass[i] = randf() * 20000.0f - 10000.0f;
    }
    for (quint64 i = 0; i < numberOfParticles * 3; ++i)
    {
        coordinates[i] = randf() * 2.0f - 1.0f;
    }
    for (quint64 i = 0; i < numberOfParticles * 3; ++i)
    {
        velocities[i] = randf() * 0.0000002f - 0.0000001f;
    }
    //calculateDistances();
}

void Gravity3DMassDifferentialNBodyEngine::initSunEarth()
{
    mass[0] = 1.9891e+30f;
    coordinates[0] = coordinates[1] = coordinates[2] = 0.0f;
    velocities[0] = velocities[1] = velocities[2] = 0.0f;

    mass[1] = 5.972e+24f;
    coordinates[3] = 1.495978e+11f;
    coordinates[4] = 0.0f;
    coordinates[5] = 0.0f;
    velocities[3] = 0.0f;
    velocities[4] = 29780.0f;
    velocities[5] = 0.0f;
}
