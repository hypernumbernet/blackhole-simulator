#pragma once

#include "abstractnbodyengine.h"

#include <QObject>
#include <QThread>
#include <QMutex>

class ThreadController : public QObject
{
    Q_OBJECT

public:
    ThreadController(QObject* = nullptr);

    void initialize(AbstractNBodyEngine* const);

signals:
    void calculateTimeProgress(int);
    void calculateInteraction(int);

private:
    QThread workerThread;
};
