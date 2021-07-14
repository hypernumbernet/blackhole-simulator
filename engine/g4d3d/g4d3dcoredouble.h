#pragma once

#include "abstractenginecoredouble.h"
#include "g4d3dcalculation.h"

class G4D3DCoreDouble : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    G4D3DCoreDouble(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new G4D3DCoreDouble(engine, threadNumber);
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
    G4D3DCalculation<double> m_calc;
};
