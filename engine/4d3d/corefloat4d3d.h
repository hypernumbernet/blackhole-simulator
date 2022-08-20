#pragma once

#include "engine/abstractenginecorefloat.h"
#include "calculation4d3d.h"

class CoreFloat4D3D : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    CoreFloat4D3D(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloat4D3D(engine, threadNumber);
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
    Calculation4D3D<float> m_calc;
};
