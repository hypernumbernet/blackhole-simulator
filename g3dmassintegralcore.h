#pragma once

#include "abstractenginecore.h"
#include "g3dmassintegralnbe.h"

class G3DMassIntegralCore : public AbstractEngineCore
{
    Q_OBJECT

public:
    explicit G3DMassIntegralCore(AbstractNBodyEngine* const, QObject* = nullptr);

public slots:
    void calculateTimeProgress(int threadNumber) const;
    void calculateInteraction(int threadNumber) const;
};
