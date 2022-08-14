#pragma once

#include "engine/abstractenginecorefloat.h"
#include "calculationg3d4d.h"

class CoreFloatG3D4D : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    CoreFloatG3D4D(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloatG3D4D(engine, threadNumber);
    }

    bool hasRangeInteraction() const override
    {
        return m_hasRangeTimeProgress;
    }

public slots:
    inline void calculateTimeProgress() const override
    {
        m_calc.calculateTimeProgress();
        resultReady();
    }

    inline void calculateInteraction() const override
    {
        m_calc.calculateInteraction();
        resultReady();
    }

private:
    CalculationG3D4D<float> m_calc;
};
