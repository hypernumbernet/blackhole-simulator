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

QDebug operator<<(QDebug q, const Quaternion& x)
{
    std::ostringstream o;
    o << x;
    q << o.str().c_str();
    return q;
}

QDebug operator<<(QDebug q, const std::complex<double>& x)
{
    std::ostringstream o;
    o << x;
    q << o.str().c_str();
    return q;
}

QDebug operator<<(QDebug q, const Biquaternion& x)
{
    std::ostringstream o;
    o << x;
    q << o.str().c_str();
    return q;
}
