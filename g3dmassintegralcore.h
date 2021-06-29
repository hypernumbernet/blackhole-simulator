#pragma once

#include "abstractenginecoresingle.h"
#include "g3dmassintegralnbe.h"

class G3DMassIntegralCore : public AbstractEngineCoreSingle
{
    Q_OBJECT

public:
    explicit G3DMassIntegralCore(AbstractNBodyEngine<float>* const, QObject* = nullptr);

public slots:
    void calculateTimeProgress(int threadNumber) const;
    void calculateInteraction(int threadNumber) const;
};
