#pragma once

#include "abstractenginecorefloat.h"
#include "g4d3dcalculation.h"

class G4D3DCoreFloat : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    G4D3DCoreFloat(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new G4D3DCoreFloat(engine, threadNumber);
    }

public slots:
    inline void calculateTimeProgress() const
    {
        m_calc.calculateTimeProgress();
        resultReady();
    }

    inline void calculateInteraction() const
    {
        m_calc.calculateInteraction();
        resultReady();
    }

private:
    G4D3DCalculation<float> m_calc;
};
