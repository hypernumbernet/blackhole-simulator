#pragma once

#include "updateui.h"
#include "threadcontroller.h"
#include "abstractnbodyengine.h"

#include "g3dmassdiffnbe.h"
#include "g3dmassdiffcorefloat.h"
#include "g3dmassdiffcoredouble.h"
#include "g3dmassintegralnbe.h"
#include "g3dmassintegralcorefloat.h"
#include "g3dmassintegralcoredouble.h"
#include "g3d4dmassdiffnbe.h"
#include "g3d4dmassdiffcorefloat.h"
#include "g3d4dmassdiffcoredouble.h"

#include <QObject>
#include <QBasicTimer>
#include <QThread>
#include <QTimerEvent>

class ThreadAdmin : public QThread
{
    Q_OBJECT

public:
    explicit ThreadAdmin(QObject* = nullptr);
    ~ThreadAdmin();

    int frameNum();
    void startSim();
    int size() const;
    ThreadController*  at(int) const;
    void reset();

    typedef AbstractEngineCore* (*engineFactoryFloat)(AbstractNBodyEngine<float>* const);
    typedef AbstractEngineCore* (*engineFactoryDouble)(AbstractNBodyEngine<double>* const);

    void initializeFloat(AbstractNBodyEngine<float>* const, engineFactoryFloat);
    void initializeDouble(AbstractNBodyEngine<double>* const, engineFactoryDouble);

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

    QBasicTimer m_simulateTimer;
    QBasicTimer m_resetTimer;
    QBasicTimer m_frameAdvanceTimer;
    QList<ThreadController*> m_controllers;
};
