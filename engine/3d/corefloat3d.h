#pragma once

#include "engine/abstractenginecorefloat.h"

class CoreFloat3D : public AbstractEngineCoreFloat
{
    Q_OBJECT
public:
    explicit CoreFloat3D(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : AbstractEngineCoreFloat(engine, threadNumber)
    {
    }

    static inline AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreFloat3D(engine, threadNumber);
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
