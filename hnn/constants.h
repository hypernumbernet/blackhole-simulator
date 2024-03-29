#pragma once

#include <complex>

namespace hnn // https://github.com/hypernumbernet
{

constexpr inline double PI = 3.141592653589793;
constexpr inline double GRAVITATIONAL_CONSTANT = 6.6743e-11; // 2018 CODATA
constexpr inline double SPEED_OF_LIGHT = 2.99792458e+8;

constexpr double degreeToRadian(const double degree)
{
    return degree / 180. * PI;
}

template <typename T>
constexpr T hAbs(const T& t) { return t >= 0 ? t : -t; }

template <typename T>
constexpr const T& hMin(const T& a, const T& b) { return (a < b) ? a : b; }

template <typename T>
constexpr const T& hMax(const T& a, const T& b) { return (a < b) ? b : a; }

constexpr bool fuzzyCompare(const double p1, const double p2)
{
    return (hAbs(p1 - p2) * 1000000000000. <= hMin(hAbs(p1), hAbs(p2)));
}

constexpr bool fuzzyCompare(const float p1, const float p2)
{
    return (hAbs(p1 - p2) * 100000.f <= hMin(hAbs(p1), hAbs(p2)));
}

constexpr bool fuzzyCompare(const std::complex<double>& p1, const std::complex<double>& p2)
{
    return fuzzyCompare(p1.real(), p2.real()) && fuzzyCompare(p1.imag(), p2.imag());
}

} // namespace
