#pragma once

#include "engine/abstractenginecorefloat.h"
#include "calculationg3d.h"

class CoreFloatG3D : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    explicit CoreFloatG3D(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloatG3D(engine, threadNumber);
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
    CalculationG3D<float> m_calc;
};
