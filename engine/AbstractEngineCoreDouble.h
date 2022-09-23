#pragma once

#include "AbstractEngineCore.h"
#include "AbstractNBodyEngine.h"

class AbstractEngineCoreDouble : public AbstractEngineCore
{
    Q_OBJECT

public:
    AbstractEngineCoreDouble(AbstractNBodyEngine<double>* const engine, const int threadNumber)
        : m_hasRangeTimeProgress(engine->timeProgressRanges().at(threadNumber).end -
                                 engine->timeProgressRanges().at(threadNumber).start > 0)
        , m_hasRangeInteraction(engine->interactionRanges().at(threadNumber).end -
                                engine->interactionRanges().at(threadNumber).start > 0)
    {
    }

    bool hasRangeTimeProgress() const override
    {
        return m_hasRangeTimeProgress;
    }

    bool hasRangeInteraction() const override
    {
        return m_hasRangeInteraction;
    }

protected:
    bool m_hasRangeTimeProgress;
    bool m_hasRangeInteraction;
};
