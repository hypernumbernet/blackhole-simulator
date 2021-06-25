#pragma once

#include "abstractenginecore.h"

#include <QObject>
#include <QThread>
#include <QMutex>

class ThreadController : public QObject
{
    Q_OBJECT

public:
    explicit ThreadController(QObject* = nullptr);

    void initialize(AbstractEngineCore* const);
    void reset();

signals:
    void calculateTimeProgress(int);
    void calculateInteraction(int);

private:
    AbstractEngineCore* m_core;
    QThread workerThread;
};
