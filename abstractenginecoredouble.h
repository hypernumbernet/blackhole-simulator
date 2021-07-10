#pragma once

#include "abstractenginecore.h"
#include "abstractnbodyengine.h"

#include <QObject>

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

    bool hasRangeTimeProgress() const
    {
        return m_hasRangeTimeProgress;
    }

    bool hasRangeInteraction() const
    {
        return m_hasRangeInteraction;
    }

protected:
    bool m_hasRangeTimeProgress;
    bool m_hasRangeInteraction;
};
