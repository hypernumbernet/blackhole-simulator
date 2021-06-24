#pragma once

#include "updateui.h"
#include "threadcontroller.h"
#include "abstractnbodyengine.h"

#include <QObject>
#include <QBasicTimer>

class ThreadAdmin : public QObject
{
    Q_OBJECT

public:
    ThreadAdmin(UpdateUi* const updateUi, QObject* parent = nullptr)
        : QObject(parent)
        , m_updateUi(updateUi)
        , m_isSimulating(false)
        , m_threadCount(QThread::idealThreadCount())
        , m_waitForDone(0)
    {
        m_simulateTimer.start(0, this);
        for (int i = 0; i < m_threadCount; ++i) {
            m_controllers.append(new ThreadController);
        }
    }
    ~ThreadAdmin()
    {
        m_simulateTimer.stop();
    }

    void reset()
    {
        m_isSimulating = false;
        m_frameNum = 0;
    }

    int frameNum()
    {
        return m_frameNum;
    }

    void startSim()
    {
        m_isSimulating = !m_isSimulating;
        emit m_updateUi->updateStartButtonText(m_isSimulating);
    }

    void setThreadParam(AbstractNBodyEngine* const engine)
    {
//        for (int i = 0; i < m_controllers.size(); ++i) {
//            m_controllers.at(i);
//        }
//        int count = engine->threadCount();
//        m_controllers.resize(count);
//        for (int i = 0; i < count; ++i) {
//            m_controllers.append(new ThreadController(engine));
//        }
//        for (int i = 0; i < count; ++i) {
//            m_controllers.at(i).initialize(engine);
//        }
        for (int i = 0; i < m_controllers.size(); ++i) {
            m_controllers.at(i)->initialize(engine);
        }
    }

public slots:
    void frameAdvance()
    {
        if (!m_isSimulating) {
            //updateParticles();
            ++m_frameNum;
        }
    }

    void handleResults()
    {
        --m_waitForDone;
//        if (m_waitForDone == 0)
//        {
//            //emit m_updateUi->allDone();
//            //allDone();

//        }
    }

private:
    void timerEvent(QTimerEvent*) override
    {
        if (m_isSimulating && m_waitForDone == 0) {
//            updateParticles();
//            m_NBodyEngine->calculateTimeProgress();
//            m_NBodyEngine->calculateInteraction();
            for (int i = 0; i < m_controllers.size(); ++i) {
                ++m_waitForDone;
                m_controllers.at(i)->calculateTimeProgress();
            }
            ++m_frameNum;
        }
    }

    UpdateUi* const m_updateUi;
    bool m_isSimulating;
    const int m_threadCount;
    int m_waitForDone;

    QBasicTimer m_simulateTimer;
    int m_frameNum = 0;
    QVector<ThreadController*> m_controllers;
};
