#pragma once

#include "engine/abstractenginecoredouble.h"
#include "calculation4d3d.h"

class CoreDouble4D3D : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    CoreDouble4D3D(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDouble4D3D(engine, threadNumber);
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
    Calculation4D3D m_calc;
};
