#pragma once

namespace hnn // https://github.com/hypernumbernet
{

constexpr double PI = 3.141592653589793;
constexpr double GRAVITATIONAL_CONSTANT = 6.6743e-11; // 2018 CODATA
constexpr double SPEED_OF_LIGHT = 2.99792458e+8;

constexpr double degreeToRadian(const double degree)
{
    return degree / 180.0 * PI;
}

} // namespace
