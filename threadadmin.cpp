#include "threadadmin.h"

ThreadAdmin::ThreadAdmin(UpdateUi* const updateUi, QObject* parent)
    : QThread(parent)
    , m_updateUi(updateUi)
    , m_isSimulating(false)
    , m_threadCount(QThread::idealThreadCount())
    , m_waitForDone(0)
    , m_calculateNext(0)
    , m_frameNum(0)
{
    for (int i = 0; i < m_threadCount; ++i) {
        m_controllers.append(new ThreadController);
    }
    m_simulateTimer.start(0, this);
}

ThreadAdmin::~ThreadAdmin()
{
    m_resetTimer.stop();
    m_frameAdvanceTimer.stop();
    m_simulateTimer.stop();
}

void ThreadAdmin::reset(const bhs::SimCondition& sim)
{
    m_frameAdvanceTimer.stop();
    m_isSimulating = false;
    m_frameNum = 0;

    if (m_waitForDone > 0) {
        m_sim = sim;
        m_resetTimer.start(100, this);
        return;
    }
    for (int i = 0; i < m_controllers.size(); ++i) {
        m_controllers.at(i)->reset();
    }
    emit m_updateUi->resetParticles(sim);
}

int ThreadAdmin::frameNum()
{
    return m_frameNum;
}

void ThreadAdmin::startSim()
{
    m_frameAdvanceTimer.stop();
    m_isSimulating = !m_isSimulating;
    emit m_updateUi->updateStartButtonText(m_isSimulating);
}

void ThreadAdmin::frameAdvance(int count)
{
    if (!m_isSimulating) {
        m_endOfFrameAdvance = m_frameNum + count;
        m_frameAdvanceTimer.start(0, this);
    }
}

void ThreadAdmin::handleResults()
{
    --m_waitForDone;
}

void ThreadAdmin::updateParticles()
{
    if (m_waitForDone <= 0)
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
            ++m_frameNum;
        }
    }
}

void ThreadAdmin::timerEvent(QTimerEvent* ev)
{
    if (ev->timerId() == m_resetTimer.timerId()) {
        m_resetTimer.stop();
        reset(m_sim);
    } else if (ev->timerId() == m_simulateTimer.timerId() && m_isSimulating) {
        updateParticles();
    } else if (ev->timerId() == m_frameAdvanceTimer.timerId()) {
        updateParticles();
        if (m_frameNum >= m_endOfFrameAdvance)
            m_frameAdvanceTimer.stop();
    }
}

int ThreadAdmin::size() const
{
    return m_controllers.size();
}

ThreadController*  ThreadAdmin::at(int i) const
{
    return m_controllers.at(i);
}
