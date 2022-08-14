#pragma once

#include "engine/abstractenginecorefloat.h"
#include "g3dcalculation.h"

class G3DCoreFloat : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    explicit G3DCoreFloat(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new G3DCoreFloat(engine, threadNumber);
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
    G3DCalculation<float> m_calc;
};
