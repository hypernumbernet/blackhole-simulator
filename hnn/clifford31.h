#pragma once

#include <array>
#include <cmath>
#include <ostream>

// Simple 4D vector class for coordinates (x, y, z, t)
struct Vector4D {
    double x, y, z, t;
    Vector4D(double x_ = 0.0, double y_ = 0.0, double z_ = 0.0, double t_ = 0.0)
        : x(x_), y(y_), z(z_), t(t_) {}
};

// Class to handle 16-dimensional representation of Clifford Algebra Cl(3,1)
class Clifford31 {
public:
    std::array<double, 16> coeffs;

    Clifford31() : coeffs{} {}
    explicit Clifford31(const std::array<double, 16>& values) : coeffs(values) {}

    Clifford31 operator+(const Clifford31& other) const {
        Clifford31 result;
        for (size_t i = 0; i < 16; ++i) {
            result.coeffs[i] = coeffs[i] + other.coeffs[i];
        }
        return result;
    }

    Clifford31 operator-(const Clifford31& other) const {
        Clifford31 result;
        for (size_t i = 0; i < 16; ++i) {
            result.coeffs[i] = coeffs[i] - other.coeffs[i];
        }
        return result;
    }

    Clifford31 operator*(const Clifford31& other) const {
        Clifford31 result;
        result.coeffs[0] += coeffs[0] * other.coeffs[0];
        for (int i = 1; i <= 3; ++i) {
            result.coeffs[0] += coeffs[i] * other.coeffs[i];
        }
        result.coeffs[0] -= coeffs[4] * other.coeffs[4];
        result.coeffs[5] += coeffs[1] * other.coeffs[2] - coeffs[2] * other.coeffs[1];
        result.coeffs[6] += coeffs[1] * other.coeffs[3] - coeffs[3] * other.coeffs[1];
        result.coeffs[7] += coeffs[2] * other.coeffs[3] - coeffs[3] * other.coeffs[2];
        result.coeffs[8] += coeffs[4] * other.coeffs[1] - coeffs[1] * other.coeffs[4];
        result.coeffs[9] += coeffs[4] * other.coeffs[2] - coeffs[2] * other.coeffs[4];
        result.coeffs[10] += coeffs[4] * other.coeffs[3] - coeffs[3] * other.coeffs[4];
        return result;
    }

    Clifford31 operator*(double scalar) const {
        Clifford31 result;
        for (size_t i = 0; i < 16; ++i) {
            result.coeffs[i] = coeffs[i] * scalar;
        }
        return result;
    }

    bool operator==(const Clifford31& other) const {
        return coeffs == other.coeffs;
    }

    // Reverse (conjugate) of the multivector
    Clifford31 reverse() const {
        Clifford31 result = *this;
        // Reverse signs of bivectors (5-10) and trivectors (11-14)
        for (int i = 5; i <= 14; ++i) {
            result.coeffs[i] = -result.coeffs[i];
        }
        return result;
    }

    // Method to distort coordinates using bivector part as a rotor
    Vector4D distort(const Vector4D& v) const {
        // Extract bivector part and compute magnitude
        double magnitude = 0.0;
        for (int i = 5; i <= 10; ++i) {
            magnitude += coeffs[i] * coeffs[i];
        }
        magnitude = std::sqrt(magnitude);
        if (magnitude == 0.0) return v; // No distortion if bivector is zero

        // Create rotor R = cos(theta/2) + sin(theta/2) * B/|B|
        double theta = magnitude; // Angle of rotation proportional to bivector magnitude
        Clifford31 R;
        R.coeffs[0] = std::cos(theta / 2.0); // Scalar part
        double scale = std::sin(theta / 2.0) / magnitude;
        for (int i = 5; i <= 10; ++i) {
            R.coeffs[i] = coeffs[i] * scale; // Bivector part
        }

        // Convert vector to Clifford form
        Clifford31 vec;
        vec.coeffs[1] = v.x; // e1
        vec.coeffs[2] = v.y; // e2
        vec.coeffs[3] = v.z; // e3
        vec.coeffs[4] = v.t; // e0 (time)

        // Apply distortion: R * v * ~R
        Clifford31 R_conj = R.reverse();
        Clifford31 result = R * vec * R_conj;

        // Extract vector part
        return Vector4D(result.coeffs[1], result.coeffs[2], result.coeffs[3], result.coeffs[4]);
    }

    friend std::ostream& operator<<(std::ostream& os, const Clifford31& c) {
        os << "[ ";
        for (size_t i = 0; i < 16; ++i) {
            if (c.coeffs[i] != 0) {
                os << c.coeffs[i];
                if (i == 0) os << " + ";
                else if (i <= 3) os << "e" << i << " + ";
                else if (i == 4) os << "e0 + ";
                else if (i == 5) os << "e12 + ";
                else if (i == 6) os << "e13 + ";
                else if (i == 7) os << "e23 + ";
                else if (i == 8) os << "e01 + ";
                else if (i == 9) os << "e02 + ";
                else if (i == 10) os << "e03 + ";
                else if (i == 11) os << "e123 + ";
                else if (i == 12) os << "e012 + ";
                else if (i == 13) os << "e013 + ";
                else if (i == 14) os << "e023 + ";
                else if (i == 15) os << "e0123 + ";
            }
        }
        os << "]";
        return os;
    }
};

inline Clifford31 operator*(double scalar, const Clifford31& c) {
    return c * scalar;
}
