#pragma once

#include "g3dmassdiffnbe.h"

class G3DMassDiffCoreSingle : public AbstractEngineCoreSingle
{
    Q_OBJECT
public:
    explicit G3DMassDiffCoreSingle(G3DMassDiffNBE<float>* const, QObject* = nullptr);

public slots:
    void calculateTimeProgress(int threadNumber) const;
    void calculateInteraction(int threadNumber) const;

private:
    G3DMassDiffNBE<float>* const m_e;
};
