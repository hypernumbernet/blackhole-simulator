#pragma once

#include "updateui.h"
#include "threadcontroller.h"
#include "abstractnbodyengine.h"

#include <QObject>
#include <QBasicTimer>
#include <QThread>
#include <QTimerEvent>

class ThreadAdmin : public QThread
{
    Q_OBJECT

public:
    ThreadAdmin(UpdateUi* const, QObject* = nullptr);
    ~ThreadAdmin();

    int frameNum();
    void startSim();
    int size() const;
    ThreadController*  at(int) const;
    void reset(const bhs::SimCondition& sim);

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
    QBasicTimer m_resetTimer;
    QList<ThreadController*> m_controllers;
    bhs::SimCondition m_sim;
};
