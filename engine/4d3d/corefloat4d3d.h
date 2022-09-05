#pragma once

#include "engine/abstractenginecorefloat.h"

class CoreFloat4D3D : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    CoreFloat4D3D(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
    {
    }

    static AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloat4D3D(engine, threadNumber);
    }

public slots:
    void calculateTimeProgress() const
    {
        resultReady();
    }

    void calculateInteraction() const
    {
        resultReady();
    }
};
