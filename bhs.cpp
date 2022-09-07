#include "bhs.h"

double bhs::rand0to1()
{
    return (double)rand() / (double)RAND_MAX;
}

// Random numbers in the range of equality between plus and minus areas
double bhs::rand0center1max()
{
    if (rand() % 2 == 0)
        return (double)rand() / (double)RAND_MAX;
    else
        return -(double)rand() / (double)RAND_MAX;
}

QDebug operator<<(QDebug q, const std::complex<double>& c)
{
    std::ostringstream o;
    o << c;
    q << o.str().c_str();
    return q;
}
