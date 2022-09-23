#pragma once

#include "engine/AbstractEngineCoreDouble.h"
#include "CalculationUniverse2.h"

class CoreDoubleUniverse2 : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    CoreDoubleUniverse2(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDoubleUniverse2(engine, threadNumber);
    }

public slots:
    void calculateTimeProgress() const
    {
        m_calc.calculateTimeProgress();
        resultReady();
    }

    void calculateInteraction() const
    {
        m_calc.calculateInteraction();
        resultReady();
    }

private:
    CalculationUniverse2 m_calc;
};
