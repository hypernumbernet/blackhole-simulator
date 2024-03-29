#pragma once

#include "engine/AbstractEngineCoreDouble.h"
#include "Calculation3D.h"

class CoreDouble3D : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    explicit CoreDouble3D(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new CoreDouble3D(engine, threadNumber);
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
    Calculation3D m_calc;
};
