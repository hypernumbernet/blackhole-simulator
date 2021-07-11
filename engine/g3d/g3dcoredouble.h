#pragma once

#include "abstractenginecoredouble.h"
#include "g3dcalculation.h"

class G3DCoreDouble : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    explicit G3DCoreDouble(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new G3DCoreDouble(engine, threadNumber);
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
    G3DCalculation<double> m_calc;
};
