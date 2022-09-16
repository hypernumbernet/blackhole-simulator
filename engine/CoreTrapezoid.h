#pragma once

#include "abstractenginecore.h"
#include "abstractnbodyengine.h"

class CoreTrapezoid : public AbstractEngineCore
{
    Q_OBJECT

public:
    explicit CoreTrapezoid(AbstractNBodyEngine<float>* const engine, const int threadNumber)
        : m_hasRangeTimeProgress(engine->timeProgressRanges().at(threadNumber).end -
                                 engine->timeProgressRanges().at(threadNumber).start > 0)
        , m_hasRangeInteraction(engine->interactionRanges().at(threadNumber).end -
                                engine->interactionRanges().at(threadNumber).start > 0)
    {
    }

    static AbstractEngineCore* factory(AbstractNBodyEngine<float>* const engine, const int threadNumber)
    {
        return new CoreTrapezoid(engine, threadNumber);
    }

    bool hasRangeTimeProgress() const override
    {
        return m_hasRangeTimeProgress;
    }

    bool hasRangeInteraction() const override
    {
        return m_hasRangeInteraction;
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

protected:
    bool m_hasRangeTimeProgress;
    bool m_hasRangeInteraction;
};
