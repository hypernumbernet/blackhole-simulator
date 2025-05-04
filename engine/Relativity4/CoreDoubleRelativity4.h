#pragma once

#include "engine/AbstractEngineCoreDouble.h"
#include "CalculationRelativity4.h"

class CoreDoubleRelativity4 : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    CoreDoubleRelativity4(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDoubleRelativity4(engine, threadNumber);
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
    CalculationRelativity4 m_calc;
};
