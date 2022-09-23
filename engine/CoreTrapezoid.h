#pragma once

#include "AbstractEngineCore.h"

class CoreTrapezoid : public AbstractEngineCore
{
    Q_OBJECT

public:
    explicit CoreTrapezoid(const bhs::IntRange& timeProgressRange, const bhs::IntRange& interactionRange)
        : m_hasRangeTimeProgress(timeProgressRange.end - timeProgressRange.start > 0)
        , m_hasRangeInteraction(interactionRange.end - interactionRange.start > 0)
    {
    }

    static AbstractEngineCore* factory(const bhs::IntRange& timeProgressRange, const bhs::IntRange& interactionRange)
    {
        return new CoreTrapezoid(timeProgressRange, interactionRange);
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
    }

    void calculateInteraction() const override
    {
    }

protected:
    bool m_hasRangeTimeProgress;
    bool m_hasRangeInteraction;
};
