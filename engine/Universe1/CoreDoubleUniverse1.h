#pragma once

#include "engine/AbstractEngineCoreDouble.h"
#include "CalculationUniverse1.h"

class CoreDoubleUniverse1 : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    CoreDoubleUniverse1(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDoubleUniverse1(engine, threadNumber);
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
    CalculationUniverse1 m_calc;
};
