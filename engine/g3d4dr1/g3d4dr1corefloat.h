#pragma once

#include "engine/abstractenginecorefloat.h"
#include "g3d4dr1calculation.h"

class G3D4DR1CoreFloat : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    G3D4DR1CoreFloat(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new G3D4DR1CoreFloat(engine, threadNumber);
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
    G3D4DR1Calculation<float> m_calc;
};
