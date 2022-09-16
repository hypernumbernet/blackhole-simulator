#pragma once

#include "abstractenginecore.h"

class CoreEvenlyDivided : public AbstractEngineCore
{
    Q_OBJECT

public:
    explicit CoreEvenlyDivided(const bhs::IntRange& timeProgressRange)
        : m_hasRangeTimeProgress(timeProgressRange.end - timeProgressRange.start > 0)
    {
    }

    static AbstractEngineCore* factory(const bhs::IntRange& timeProgressRange, const bhs::IntRange&)
    {
        return new CoreEvenlyDivided(timeProgressRange);
    }

    bool hasRangeTimeProgress() const override
    {
        return m_hasRangeTimeProgress;
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

protected:
    bool m_hasRangeTimeProgress;
};
