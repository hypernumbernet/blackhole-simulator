#pragma once

#include "abstractenginecorefloat.h"
#include "quaternion.h"
#include "g3d4dcalculation.h"

using namespace bhs;

class G3D4DCoreFloat : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    G3D4DCoreFloat(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
        , m_calc(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new G3D4DCoreFloat(engine, threadNumber);
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
    G3D4DCalculation<float> m_calc;
};
