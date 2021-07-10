#pragma once

#include "abstractenginecorefloat.h"
#include "g3deulercalculation.h"

class G3DEulerCoreFloat : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    explicit G3DEulerCoreFloat(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new G3DEulerCoreFloat(engine, threadNumber);
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
    G3DEulerCalculation<float> m_calc;
};
