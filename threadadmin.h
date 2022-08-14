#pragma once

#include <QObject>
#include <QBasicTimer>
#include <QThread>
#include <QTimerEvent>

#include "updateui.h"
#include "threadcontroller.h"
#include "engine/abstractnbodyengine.h"
#include "computeshaders.h"

#include "engine/3d/engine3d.h"
#include "engine/3d/corefloat3d.h"
#include "engine/3d/coredouble3d.h"
#include "engine/3d4d/engine3d4d.h"
#include "engine/3d4d/corefloat3d4d.h"
#include "engine/3d4d/coredouble3d4d.h"

class ThreadAdmin : public QThread
{
    Q_OBJECT

public:
    explicit ThreadAdmin(QObject*, ComputeShaders*);
    ~ThreadAdmin();

    int frameNum();
    void startSim(int);
    int size() const;
    ThreadController* at(int) const;
    void reset();
    void setComputeDevice(bhs::Compute);

    typedef AbstractEngineCore* (*coreFactoryFloat)(AbstractNBodyEngine<float>* const, int);
    typedef AbstractEngineCore* (*coreFactoryDouble)(AbstractNBodyEngine<double>* const, int);

    void initialize(AbstractNBodyEngine<float>* const, coreFactoryFloat);
    void initialize(AbstractNBodyEngine<double>* const, coreFactoryDouble);

public slots:
    void frameAdvance(int = 1);
    void handleResults();

private:
    void updateParticles();
    void timerEvent(QTimerEvent*) override;

    const int m_threadCount;
    int m_waitForDone;
    int m_calculateNext;
    int m_frameNum;
    int m_endOfFrameAdvance;
    bhs::Compute m_compute;
    ComputeShaders* m_computeShaders;

    QBasicTimer m_simulateTimer;
    QBasicTimer m_resetTimer;
    QBasicTimer m_frameAdvanceTimer;
    QList<ThreadController*> m_controllers;
};
