#pragma once

#include "abstractenginecoresingle.h"
#include "quaternion.h"

class G3SVMassDiffCore : public AbstractEngineCoreSingle
{
    Q_OBJECT
public:
    explicit G3SVMassDiffCore(AbstractNBodyEngine<float>* const, QObject* = nullptr);

public slots:
    void calculateTimeProgress(int threadNumber) const;
    void calculateInteraction(int threadNumber) const;

private:
    const float m_cinv;
    const float m_vangle;
    const float m_vangle_inv;
};
