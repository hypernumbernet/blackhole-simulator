#pragma once

#include "abstractenginecoresingle.h"

class G3DMassDiffCore : public AbstractEngineCoreSingle
{
    Q_OBJECT
public:
    explicit G3DMassDiffCore(AbstractNBodyEngine<float>* const, QObject* = nullptr);

public slots:
    void calculateTimeProgress(int threadNumber) const;
    void calculateInteraction(int threadNumber) const;
};
