#pragma once

#include "abstractenginecoresingle.h"

#include <QObject>
#include <QThread>
#include <QMutex>

class ThreadController : public QObject
{
    Q_OBJECT

public:
    explicit ThreadController(QObject* = nullptr);
    ~ThreadController();

    void initialize(AbstractEngineCoreSingle* const);
    void reset();
    bool hasRangeTimeProgress(int) const;
    bool hasRangeInteraction(int) const;

signals:
    void calculateTimeProgress(int);
    void calculateInteraction(int);

private:
    AbstractEngineCoreSingle* m_core;
    QThread workerThread;
};
