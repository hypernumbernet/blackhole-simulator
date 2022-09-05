#pragma once

#include "engine/abstractenginecorefloat.h"

class CoreFloat3D4DR1 : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    CoreFloat3D4DR1(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
    {
    }

    static AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloat3D4DR1(engine, threadNumber);
    }

    bool hasRangeInteraction() const override
    {
        return m_hasRangeTimeProgress;
    }

public slots:
    void calculateTimeProgress() const override
    {
        resultReady();
    }

    void calculateInteraction() const override
    {
        resultReady();
    }
};
