#pragma once

#include "engine/abstractenginecorefloat.h"

class CoreFloatRelativity2 : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    CoreFloatRelativity2(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
    {
    }

    static AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloatRelativity2(engine, threadNumber);
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
