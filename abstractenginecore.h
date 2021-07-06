#pragma once

#include "updateui.h"

#include <QObject>

class AbstractEngineCore : public QObject
{
    Q_OBJECT
public:
    explicit AbstractEngineCore(){}
    virtual ~AbstractEngineCore(){}

    virtual bool hasRangeTimeProgress() const = 0;
    virtual bool hasRangeInteraction() const = 0;

protected:
    void resultReady() const
    {
        emit UpdateUi::it().resultReady();
    }

public slots:
    virtual void calculateTimeProgress() const = 0;
    virtual void calculateInteraction() const = 0;
};
