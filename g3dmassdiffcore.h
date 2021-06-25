#pragma once

#include "abstractenginecore.h"

class G3DMassDiffCore : public AbstractEngineCore
{
    Q_OBJECT
public:
    explicit G3DMassDiffCore(AbstractNBodyEngine* const, QObject* = nullptr);

public slots:
    void calculateTimeProgress(int threadNumber) const;
    void calculateInteraction(int threadNumber) const;

};
