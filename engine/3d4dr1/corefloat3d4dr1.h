#pragma once

#include "engine/abstractenginecorefloat.h"
#include "calculation3d4dr1.h"

class CoreFloat3D4DR1 : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    CoreFloat3D4DR1(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloat3D4DR1(engine, threadNumber);
    }

    bool hasRangeInteraction() const override
    {
        return m_hasRangeTimeProgress;
    }

public slots:
    void calculateTimeProgress() const override
    {
        m_calc.calculateTimeProgress();
        resultReady();
    }

    void calculateInteraction() const override
    {
        m_calc.calculateInteraction();
        resultReady();
    }

private:
    Calculation3D4DR1<float> m_calc;
};
