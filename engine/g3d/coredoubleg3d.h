#pragma once

#include "engine/abstractenginecoredouble.h"
#include "calculationg3d.h"

class CoreDoubleG3D : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    explicit CoreDoubleG3D(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDoubleG3D(engine, threadNumber);
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
    CalculationG3D<double> m_calc;
};
