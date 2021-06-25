#pragma once

#include "abstractnbodyengine.h"

#include <QObject>
#include <QThread>

class ThreadController : public QObject
{
    Q_OBJECT

public:
    ThreadController(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    void initialize(AbstractNBodyEngine* const engine)
    {
        engine->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, engine, &QObject::deleteLater);
        connect(this, &ThreadController::calculateTimeProgress, engine, &AbstractNBodyEngine::calculateTimeProgress);
        connect(this, &ThreadController::calculateInteraction, engine, &AbstractNBodyEngine::calculateInteraction);
        //connect(engine, &AbstractNBodyEngine::resultReady, this, &ThreadController::handleResults);
        workerThread.start();
    }

//public slots:
//    void handleResults()
//    {
//        //--waitForDone;
//    }

signals:
    void calculateTimeProgress(int);
    void calculateInteraction(int);

private:
    QThread workerThread;

};
