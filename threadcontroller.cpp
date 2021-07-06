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
    if (m_core)
        m_core->deleteLater();
    m_core = nullptr;
}

bool ThreadController::hasRangeTimeProgress() const
{
    return m_core->hasRangeTimeProgress();
}

bool ThreadController::hasRangeInteraction() const
{
    return m_core->hasRangeInteraction();
}
