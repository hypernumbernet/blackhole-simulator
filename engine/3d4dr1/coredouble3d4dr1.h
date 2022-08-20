#pragma once

#include "engine/abstractenginecoredouble.h"
#include "calculation3d4dr1.h"

class CoreDouble3D4DR1 : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    CoreDouble3D4DR1(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDouble3D4DR1(engine, threadNumber);
    }

    bool hasRangeInteraction() const override
    {
        return m_hasRangeTimeProgress;
    }

public slots:
    void calculateTimeProgress() const override
    {
        m_calc.calculateTimeProgress();
        resultReady();
    }

    void calculateInteraction() const override
    {
        m_calc.calculateInteraction();
        resultReady();
    }

private:
    Calculation3D4DR1<double> m_calc;
};
