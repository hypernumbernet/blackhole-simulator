#pragma once

#include "updateui.h"
#include "threadcontroller.h"
#include "abstractnbodyengine.h"
#include "computeshaders.h"

#include "g3dmasseulerengine.h"
#include "g3dmasseulercorefloat.h"
#include "g3dmasseulercoredouble.h"
#include "g3d4dmasseulerengine.h"
#include "g3d4dmasseulercorefloat.h"
#include "g3d4dmasseulercoredouble.h"

#include <QObject>
#include <QBasicTimer>
#include <QThread>
#include <QTimerEvent>

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

    typedef AbstractEngineCore* (*engineFactoryFloat)(AbstractNBodyEngine<float>* const, int);
    typedef AbstractEngineCore* (*engineFactoryDouble)(AbstractNBodyEngine<double>* const, int);

    void initialize(AbstractNBodyEngine<float>* const, engineFactoryFloat);
    void initialize(AbstractNBodyEngine<double>* const, engineFactoryDouble);

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
