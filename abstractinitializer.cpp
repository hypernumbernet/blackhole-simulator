#include "abstractinitializer.h"

AbstractInitializer::AbstractInitializer(const bhs::SimCondition& sim)
{
    m_massAverage = sim.mass;
}

//AbstractInitializer::~AbstractInitializer()
//{
//}
