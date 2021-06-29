#pragma once

#include "updateui.h"

#include <QObject>

class AbstractEngineCore : public QObject
{
    Q_OBJECT
public:
    explicit AbstractEngineCore(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    virtual bool hasRangeTimeProgress(const int threadNum) const = 0;
    virtual bool hasRangeInteraction(const int threadNum) const = 0;

protected:
    void resultReady() const
    {
        emit UpdateUi::it().resultReady();
    }

public slots:
    virtual void calculateTimeProgress(int threadNumber) const = 0;
    virtual void calculateInteraction(int threadNumber) const = 0;
};
