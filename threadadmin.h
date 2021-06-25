#pragma once

#include "updateui.h"
#include "threadcontroller.h"
#include "abstractnbodyengine.h"

#include <QObject>
#include <QBasicTimer>
#include <QMutex>

class ThreadAdmin : public QObject
{
    Q_OBJECT

public:
    ThreadAdmin(UpdateUi* const, QObject* = nullptr);
    ~ThreadAdmin();

    void reset();
    int frameNum();
    void startSim();
    void setThreadParam(AbstractEngineCore* const);

public slots:
    void frameAdvance();
    void handleResults();

private:
    void updateParticles();
    void timerEvent(QTimerEvent*) override;

    UpdateUi* const m_updateUi;
    bool m_isSimulating;
    const int m_threadCount;
    int m_waitForDone;
    int m_calculateNext;
    int m_frameNum;

    QBasicTimer m_simulateTimer;
    QList<ThreadController*> m_controllers;
};
