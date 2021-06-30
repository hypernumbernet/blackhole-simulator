#include "threadadmin.h"

ThreadAdmin::ThreadAdmin(QObject* parent)
    : QThread(parent)
    , m_threadCount(QThread::idealThreadCount())
    , m_waitForDone(0)
    , m_calculateNext(0)
    , m_frameNum(0)
{
    for (int i = 0; i < m_threadCount; ++i) {
        m_controllers.append(new ThreadController(this));
    }
}

ThreadAdmin::~ThreadAdmin()
{
    m_resetTimer.stop();
    m_frameAdvanceTimer.stop();
    m_simulateTimer.stop();
}

void ThreadAdmin::reset()
{
    m_frameAdvanceTimer.stop();
    m_simulateTimer.stop();
    m_frameNum = 0;

    if (m_waitForDone > 0) {
        m_resetTimer.start(100, this);
        return;
    }
    for (int i = 0; i < m_controllers.size(); ++i) {
        m_controllers.at(i)->reset();
    }
    emit UpdateUi::it().resetParticles();
}

int ThreadAdmin::frameNum()
{
    return m_frameNum;
}

void ThreadAdmin::startSim()
{
    m_frameAdvanceTimer.stop();
    if (m_simulateTimer.isActive()) {
        m_simulateTimer.stop();
    } else {
        m_simulateTimer.start(0, this);
    }

    emit UpdateUi::it().updateStartButtonText(m_simulateTimer.isActive());
}

void ThreadAdmin::frameAdvance(int count)
{
    if (!m_simulateTimer.isActive()) {
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
                if (m_controllers.at(i)->hasRangeTimeProgress(i)) {
                    ++m_waitForDone;
                    emit m_controllers.at(i)->calculateTimeProgress(i);
                }
            }
        } else {
            m_calculateNext = 0;
            for (int i = 0; i < m_controllers.size(); ++i) {
                if (m_controllers.at(i)->hasRangeInteraction(i)) {
                    ++m_waitForDone;
                    emit m_controllers.at(i)->calculateInteraction(i);
                }
            }
            ++m_frameNum;
        }
    }
}

void ThreadAdmin::timerEvent(QTimerEvent* ev)
{
    if (ev->timerId() == m_resetTimer.timerId()) {
        m_resetTimer.stop();
        reset();
    } else if (ev->timerId() == m_simulateTimer.timerId() && m_simulateTimer.isActive()) {
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

ThreadController* ThreadAdmin::at(int i) const
{
    return m_controllers.at(i);
}

void ThreadAdmin::initializeFloat(AbstractNBodyEngine<float>* const engine, initFactoryFloat factory)
{
    for (int i = 0; i < size(); ++i) {
        at(i)->initialize(factory(engine));
    }
}

void ThreadAdmin::initializeDouble(AbstractNBodyEngine<double>* const engine, initFactoryDouble factory)
{
    for (int i = 0; i < size(); ++i) {
        at(i)->initialize(factory(engine));
    }
}
