#pragma once

#include "abstractenginecoredouble.h"
#include "g3d4dr1calculation.h"

class G3D4DR1CoreDouble : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    G3D4DR1CoreDouble(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new G3D4DR1CoreDouble(engine, threadNumber);
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
    G3D4DR1Calculation<double> m_calc;
};
