#pragma once

#include "abstractnbodyengine.h"

#include <QObject>
#include <QThread>
#include <QBasicTimer>

class CalculateTimeProgress : public QObject
{
    Q_OBJECT

public:

    CalculateTimeProgress(
            const AbstractNBodyEngine::IntRange,
            QObject* parent = nullptr);

    void calculate()
    {
        for (quint64 i = m_range.start; i < m_range.end; ++i)
        {
            quint64 j = i * 3;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame; ++j;
            m_coordinates[j] += m_velocities[j] * m_timePerFrame;
        }
        emit resultReady();
    }

    void calculateInteraction()
    {
        float d1, d2, d3, distance, inv, theta;
        quint64 i, j, k, a, b;
        float time_g = m_timePerFrame * AbstractNBodyEngine::GRAVITATIONAL_CONSTANT;

        k = m_range.start * m_range.numberOfParticles - (m_range.start + 1) * m_range.start / 2;

        for (i = m_range.start; i < m_range.end; ++i)
        {
            for (j = i + 1; j < m_range.numberOfParticles; ++j)
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
                theta = inv * inv * time_g;

                d1 *= inv * theta;
                d2 *= inv * theta;
                d3 *= inv * theta;

                m_velocities[a] -= d1 * m_masses[j];
                m_velocities[a + 1] -= d2 * m_masses[j];
                m_velocities[a + 2] -= d3 * m_masses[j];
                m_velocities[b] += d1 * m_masses[i];
                m_velocities[b + 1] += d2 * m_masses[i];
                m_velocities[b + 2] += d3 * m_masses[i];

                ++k;
            }
        }
    }

signals:
    void resultReady();

private:
    const AbstractNBodyEngine::IntRange m_range;
    float* m_coordinates;
    float* m_velocities;
    float* m_masses;
    float m_timePerFrame;
};

class Controller : public QObject
{
    Q_OBJECT

public:

    //static inline int waitForDone = 0;

    Controller(const AbstractNBodyEngine::IntRange range)
    {
        auto *worker = new CalculateTimeProgress(range);
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::calculate, worker, &CalculateTimeProgress::calculate);
        connect(this, &Controller::calculateInteraction, worker, &CalculateTimeProgress::calculateInteraction);
        connect(worker, &CalculateTimeProgress::resultReady, this, &Controller::handleResults);
        workerThread.start();
    }
    ~Controller()
    {
        workerThread.quit();
        workerThread.wait();
    }

public slots:
    void handleResults()
    {
        //--waitForDone;
    }

signals:
    void calculate();
    void calculateInteraction();

private:
    QThread workerThread;

};

