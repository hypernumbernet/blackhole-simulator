#pragma once

#include "engine/abstractenginecorefloat.h"
#include "calculationg4d3d.h"

class CoreFloatG4D3D : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    CoreFloatG4D3D(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloatG4D3D(engine, threadNumber);
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
    CalculationG4D3D<float> m_calc;
};
