#pragma once

#include "engine/abstractenginecorefloat.h"
#include "calculationg3d4dr1.h"

class CoreFloatG3D4DR1 : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    CoreFloatG3D4DR1(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloatG3D4DR1(engine, threadNumber);
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
    CalculationG3D4DR1<float> m_calc;
};
