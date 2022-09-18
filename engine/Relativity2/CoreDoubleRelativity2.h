#pragma once

#include "engine/abstractenginecoredouble.h"
#include "CalculationRelativity2.h"

class CoreDoubleRelativity2 : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    CoreDoubleRelativity2(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDoubleRelativity2(engine, threadNumber);
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
    CalculationRelativity2 m_calc;
};
