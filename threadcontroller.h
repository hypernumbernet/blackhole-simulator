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
    bool hasRangeTimeProgress() const;
    bool hasRangeInteraction() const;

signals:
    void calculateTimeProgress();
    void calculateInteraction();

private:
    AbstractEngineCore* m_core = nullptr;
    QThread workerThread;
};
