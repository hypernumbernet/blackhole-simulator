#pragma once

#include "engine/abstractenginecorefloat.h"
#include "calculation3d.h"

class CoreFloat3D : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    explicit CoreFloat3D(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloat3D(engine, threadNumber);
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
    Calculation3D<float> m_calc;
};
