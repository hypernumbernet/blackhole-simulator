#pragma once

#include "engine/abstractenginecoredouble.h"
#include "calculationg4d3d.h"

class CoreDoubleG4D3D : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    CoreDoubleG4D3D(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDoubleG4D3D(engine, threadNumber);
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
    CalculationG4D3D<double> m_calc;
};
