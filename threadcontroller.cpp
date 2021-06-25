#include "threadcontroller.h"

ThreadController::ThreadController(QObject* parent)
    : QObject(parent)
{
}

void ThreadController::initialize(AbstractNBodyEngine* const engine)
{
    engine->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, engine, &QObject::deleteLater);
    connect(this, &ThreadController::calculateTimeProgress, engine, &AbstractNBodyEngine::calculateTimeProgress);
    connect(this, &ThreadController::calculateInteraction, engine, &AbstractNBodyEngine::calculateInteraction);
    workerThread.start();
}
