#pragma once

#include "engine/abstractenginecoredouble.h"
#include "CalculationGravityElasticity.h"

class CoreDoubleGravityElasticity : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    explicit CoreDoubleGravityElasticity(
            AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(
            AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDoubleGravityElasticity(engine, threadNumber);
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
    CalculationGravityElasticity m_calc;
};
