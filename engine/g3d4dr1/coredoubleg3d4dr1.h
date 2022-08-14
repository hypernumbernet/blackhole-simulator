#pragma once

#include "engine/abstractenginecoredouble.h"
#include "calculationg3d4dr1.h"

class CoreDoubleG3D4DR1 : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    CoreDoubleG3D4DR1(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDoubleG3D4DR1(engine, threadNumber);
    }

    bool hasRangeInteraction() const override
    {
        return m_hasRangeTimeProgress;
    }

public slots:
    inline void calculateTimeProgress() const override
    {
        m_calc.calculateTimeProgress();
        resultReady();
    }

    inline void calculateInteraction() const override
    {
        m_calc.calculateInteraction();
        resultReady();
    }

private:
    CalculationG3D4DR1<double> m_calc;
};
