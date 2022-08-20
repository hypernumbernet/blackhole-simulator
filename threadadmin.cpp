#include "threadadmin.h"
#include <QTimerEvent>

ThreadAdmin::ThreadAdmin(QObject* parent, ComputeShaders* cs)
    : QThread(parent)
    , m_threadCount(QThread::idealThreadCount())
    , m_waitForDone(0)
    , m_calculateNext(0)
    , m_frameNum(0)
    , m_computeShaders(cs)
{
    for (int i = 0; i < m_threadCount; ++i)
    {
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

    if (m_waitForDone > 0)
    {
        m_resetTimer.start(100, this);
        return;
    }
    for (int i = 0; i < m_controllers.size(); ++i)
    {
        at(i)->reset();
    }
    emit UpdateUi::it().resetParticles();
}

int ThreadAdmin::frameNum()
{
    return m_frameNum;
}

void ThreadAdmin::startSim(const int msec)
{
    m_frameAdvanceTimer.stop();
    if (m_simulateTimer.isActive())
        m_simulateTimer.stop();
    else
        m_simulateTimer.start(msec, this);

    emit UpdateUi::it().updateStartButtonText(m_simulateTimer.isActive());
}

void ThreadAdmin::frameAdvance(int count)
{
    if (!m_simulateTimer.isActive())
    {
        if (!m_frameAdvanceTimer.isActive())
        {
            m_endOfFrameAdvance = m_frameNum + count;
            m_frameAdvanceTimer.start(0, this);
        } else {
            m_frameAdvanceTimer.stop();
        }
    }
}

void ThreadAdmin::handleResults()
{
    --m_waitForDone;
}

void ThreadAdmin::updateParticles()
{
    if (m_compute == bhs::Compute::GPU)
    {
        m_computeShaders->update();
        ++m_frameNum;
        return;
    }
    if (m_waitForDone <= 0)
    {
        if (m_calculateNext == 0)
        {
            m_calculateNext = 1;
            for (int i = 0; i < m_controllers.size(); ++i)
            {
                if (at(i)->hasRangeTimeProgress())
                {
                    ++m_waitForDone;
                    emit at(i)->calculateTimeProgress();
                }
            }
        } else {
            m_calculateNext = 0;
            for (int i = 0; i < m_controllers.size(); ++i)
            {
                if (at(i)->hasRangeInteraction())
                {
                    ++m_waitForDone;
                    emit at(i)->calculateInteraction();
                }
            }
            ++m_frameNum;
        }
    }
}

void ThreadAdmin::timerEvent(QTimerEvent* ev)
{
    if (ev->timerId() == m_resetTimer.timerId())
    {
        m_resetTimer.stop();
        reset();
    }
    else if (ev->timerId() == m_simulateTimer.timerId() && m_simulateTimer.isActive())
    {
        updateParticles();
    }
    else if (ev->timerId() == m_frameAdvanceTimer.timerId())
    {
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

void ThreadAdmin::initialize(AbstractNBodyEngine<float>* const engine, coreFactoryFloat factory)
{
    for (int i = 0; i < size(); ++i)
    {
        at(i)->initialize(factory(engine, i));
    }
    m_computeShaders->bind(engine);
}

void ThreadAdmin::initialize(AbstractNBodyEngine<double>* const engine, coreFactoryDouble factory)
{
    for (int i = 0; i < size(); ++i)
    {
        at(i)->initialize(factory(engine, i));
    }
    m_computeShaders->bind(engine);
}

void ThreadAdmin::setComputeDevice(bhs::Compute cmp)
{
    m_compute = cmp;
}
