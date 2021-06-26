#pragma once

#include "abstractenginecore.h"
#include "quaternion.h"

class G3SVMassDiffCore : public AbstractEngineCore
{
    Q_OBJECT
public:
    explicit G3SVMassDiffCore(AbstractNBodyEngine* const, QObject* = nullptr);

public slots:
    void calculateTimeProgress(int threadNumber) const;
    void calculateInteraction(int threadNumber) const;

private:
    const float m_cinv;
    const float m_vangle;
    const float m_vangle_inv;
};
