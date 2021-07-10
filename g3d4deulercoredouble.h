#pragma once

#include "abstractenginecoredouble.h"
#include "quaternion.h"
#include "g3d4deulercalculation.h"

using namespace bhs;

class G3D4DEulerCoreDouble : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    G3D4DEulerCoreDouble(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : AbstractEngineCoreDouble(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<double>* const engine, const int threadNumber)
    {
        return new G3D4DEulerCoreDouble(engine, threadNumber);
    }

public slots:
    inline void calculateTimeProgress() const
    {
        m_calc.calculateTimeProgress();
        resultReady();
    }

    inline void calculateInteraction() const
    {
        m_calc.calculateInteraction();
        resultReady();
    }

private:
    G3D4DEulerCalculation<double> m_calc;
};
