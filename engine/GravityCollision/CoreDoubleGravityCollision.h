#pragma once

#include "engine/AbstractEngineCoreDouble.h"
#include "CalculationGravityCollision.h"

class CoreDoubleGravityCollision : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    explicit CoreDoubleGravityCollision(
            AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static AbstractEngineCore* factory(
            AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDoubleGravityCollision(engine, threadNumber);
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
    CalculationGravityCollision m_calc;
};
