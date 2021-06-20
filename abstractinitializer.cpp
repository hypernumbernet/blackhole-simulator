#include "abstractinitializer.h"

AbstractInitializer::AbstractInitializer(const bhs::SimCondition& sim, AbstractNBodyEngine* const engine)
    : m_sim(sim)
    , m_engine(engine)
{
}
