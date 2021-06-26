#include "threadcontroller.h"

ThreadController::ThreadController(QObject* parent)
    : QObject(parent)
{
}

ThreadController::~ThreadController()
{
    workerThread.quit();
    workerThread.wait();
}

void ThreadController::initialize(AbstractEngineCore* const core)
{
    m_core = core;
    core->moveToThread(&workerThread);
    connect(this, &ThreadController::calculateTimeProgress, core, &AbstractEngineCore::calculateTimeProgress);
    connect(this, &ThreadController::calculateInteraction, core, &AbstractEngineCore::calculateInteraction);
    workerThread.start();
}

void ThreadController::reset()
{
    m_core->deleteLater();
}

bool ThreadController::hasRangeTimeProgress(int threadNum) const
{
    return m_core->hasRangeTimeProgress(threadNum);
}

bool ThreadController::hasRangeInteraction(int threadNum) const
{
    return m_core->hasRangeInteraction(threadNum);
}
