#pragma once

#include "engine/abstractenginecoredouble.h"
#include "CalculationRelativity3.h"

class CoreDoubleRelativity3 : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    CoreDoubleRelativity3(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDoubleRelativity3(engine, threadNumber);
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
    CalculationRelativity3 m_calc;
};
