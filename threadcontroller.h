#pragma once

#include "abstractenginecore.h"
#include "abstractenginecorefloat.h"
#include "abstractenginecoredouble.h"

#include <QObject>
#include <QThread>
#include <QMutex>

class ThreadController : public QObject
{
    Q_OBJECT

public:
    explicit ThreadController(QObject* = nullptr);
    ~ThreadController();

    void initialize(AbstractEngineCore* const);
    void reset();
    bool hasRangeTimeProgress(int) const;
    bool hasRangeInteraction(int) const;

signals:
    void calculateTimeProgress(int);
    void calculateInteraction(int);

private:
    AbstractEngineCore* m_core = nullptr;
    QThread workerThread;
};
