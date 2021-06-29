#pragma once

#include "abstractenginecoredouble.h"

class G3DMassDiffCoreDouble : public AbstractEngineCoreDouble
{
    Q_OBJECT
public:
    explicit G3DMassDiffCoreDouble(AbstractNBodyEngine<double>* const, QObject* = nullptr);

public slots:
    void calculateTimeProgress(int threadNumber) const;
    void calculateInteraction(int threadNumber) const;
};
