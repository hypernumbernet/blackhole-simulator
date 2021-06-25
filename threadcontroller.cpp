#include "threadcontroller.h"

ThreadController::ThreadController(QObject* parent)
    : QObject(parent)
{
}

void ThreadController::initialize(AbstractEngineCore* const core)
{
    m_core = core;
    core->moveToThread(&workerThread);
    //connect(&workerThread, &QThread::finished, engine, &QObject::deleteLater);
    connect(this, &ThreadController::calculateTimeProgress, core, &AbstractEngineCore::calculateTimeProgress);
    connect(this, &ThreadController::calculateInteraction, core, &AbstractEngineCore::calculateInteraction);
    workerThread.start();
}

void ThreadController::reset()
{
//    workerThread.quit();
//    workerThread.wait();
    m_core->deleteLater();
    //delete m_core;
}
