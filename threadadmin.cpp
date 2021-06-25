#include "threadadmin.h"

ThreadAdmin::ThreadAdmin(UpdateUi* const updateUi, QObject* parent)
    : QObject(parent)
    , m_updateUi(updateUi)
    , m_isSimulating(false)
    , m_threadCount(QThread::idealThreadCount())
    , m_waitForDone(0)
    , m_calculateNext(0)
    , m_frameNum(0)
{
    m_simulateTimer.start(0, this);
    for (int i = 0; i < m_threadCount; ++i) {
        m_controllers.append(new ThreadController);
    }
}

ThreadAdmin::~ThreadAdmin()
{
    m_simulateTimer.stop();
}

void ThreadAdmin::reset()
{
    m_isSimulating = false;
    m_frameNum = 0;
    while (m_waitForDone > 0) {
        QThread::msleep(100);
    }
    for (int i = 0; i < m_controllers.size(); ++i) {
        m_controllers.at(i)->reset();
    }
}

int ThreadAdmin::frameNum()
{
    return m_frameNum;
}

void ThreadAdmin::startSim()
{
    m_isSimulating = !m_isSimulating;
    emit m_updateUi->updateStartButtonText(m_isSimulating);
}

void ThreadAdmin::setThreadParam(AbstractEngineCore* const core)
{
    for (int i = 0; i < m_controllers.size(); ++i) {
        m_controllers.at(i)->initialize(core);
    }
}

void ThreadAdmin::frameAdvance()
{
    if (!m_isSimulating) {
        updateParticles();
    }
}

void ThreadAdmin::handleResults()
{
    --m_waitForDone;
}

void ThreadAdmin::updateParticles()
{
    if (m_calculateNext == 0)
    {
        m_calculateNext = 1;
        for (int i = 0; i < m_controllers.size(); ++i) {
            ++m_waitForDone;
            emit m_controllers.at(i)->calculateTimeProgress(i);
        }
    } else {
        m_calculateNext = 0;
        for (int i = 0; i < m_controllers.size(); ++i) {
            ++m_waitForDone;
            emit m_controllers.at(i)->calculateInteraction(i);
        }
    }
    ++m_frameNum;
}

void ThreadAdmin::timerEvent(QTimerEvent*)
{
    if (m_isSimulating && m_waitForDone == 0) {
        updateParticles();
    }
}
