#pragma once

#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace hnn // https://github.com/hypernumbernet
{

// Structure to hold the result of a basis element product: index and sign
struct Product {
    uint8_t index; // Index of the resulting basis element (0 to 15)
    int8_t sign;   // Sign of the product (+1 or -1)
};

// Compute the product of two basis elements represented by bit patterns
constexpr Product compute_product(uint8_t a, uint8_t b, std::array<int8_t, 4> signature) {
    uint8_t result = a ^ b; // XOR for the symmetric difference
    int8_t sign = 1;

    // Calculate the sign based on the number of swaps needed
    for (int k = 0; k < 4; ++k) {
        if (b & (1 << k)) {
            for (int m = k + 1; m < 4; ++m) {
                if (a & (1 << m)) {
                    sign *= -1; // Each pair of anticommuting generators introduces a sign flip
                }
            }
        }
    }

    // Apply the signature for squares
    for (int k = 0; k < 4; ++k) {
        if ((a & (1 << k)) && (b & (1 << k))) {
            sign *= signature[k];
            result &= ~(1 << k); // Remove the squared generator
        }
    }

    return {result, sign};
}

// Create the multiplication table at compile time
constexpr std::array<std::array<Product, 16>, 16> make_multiplication_table(std::array<int8_t, 4> signature) {
    std::array<std::array<Product, 16>, 16> table{};
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            table[i][j] = compute_product(i, j, signature);
        }
    }
    return table;
}

// Header-only class for the Clifford algebra Cl(1,3) with signature (+,-,-,-)
class Clifford13 {
public:

    // Signature of the quadratic form: (+,-,-,-)
    static constexpr std::array<int8_t, 4> signature = {1, -1, -1, -1};

    // Array to hold coefficients for the 16 basis elements
    using CoeffArray = std::array<double, 16>;

    CoeffArray coeffs; // Coefficients of the multivector

    // Default constructor: initializes all coefficients to zero
    Clifford13() : coeffs{} {}

    // Constructor from an array of coefficients
    explicit Clifford13(CoeffArray coeffs) : coeffs(coeffs) {}

    // Addition of two multivectors
    Clifford13 operator+(const Clifford13& other) const {
        CoeffArray new_coeffs;
        for (size_t i = 0; i < 16; ++i) {
            new_coeffs[i] = coeffs[i] + other.coeffs[i];
        }
        return Clifford13(new_coeffs);
    }

    // Subtraction of two multivectors
    Clifford13 operator-(const Clifford13& other) const {
        CoeffArray new_coeffs;
        for (size_t i = 0; i < 16; ++i) {
            new_coeffs[i] = coeffs[i] - other.coeffs[i];
        }
        return Clifford13(new_coeffs);
    }

    // Scalar multiplication
    Clifford13 operator*(double scalar) const {
        CoeffArray new_coeffs;
        for (size_t i = 0; i < 16; ++i) {
            new_coeffs[i] = coeffs[i] * scalar;
        }
        return Clifford13(new_coeffs);
    }

    // Geometric product of two multivectors
    Clifford13 operator*(const Clifford13& other) const {
        CoeffArray new_coeffs{};
        for (size_t i = 0; i < 16; ++i) {
            for (size_t j = 0; j < 16; ++j) {
                auto [k, s] = multiplication_table[i][j];
                new_coeffs[k] += coeffs[i] * other.coeffs[j] * s;
            }
        }
        return Clifford13(new_coeffs);
    }

    // Get the coefficient of a specific basis element
    double get_coeff(size_t index) const {
        return (index < 16) ? coeffs[index] : 0.0;
    }

    // Set the coefficient of a specific basis element
    void set_coeff(size_t index, double value) {
        if (index < 16) {
            coeffs[index] = value;
        }
    }

    // Create a multivector representing a basis element
    static Clifford13 basis(size_t index) {
        Clifford13 mv;
        if (index < 16) {
            mv.coeffs[index] = 1.0;
        }
        return mv;
    }

    // Output stream operator for debugging
    friend std::ostream& operator<<(std::ostream& os, const Clifford13& mv) {
        bool first = true;
        const char* labels[] = {
            "1", "e0", "e1", "e2", "e3",
            "e0e1", "e0e2", "e0e3", "e1e2", "e1e3", "e2e3",
            "e0e1e2", "e0e1e3", "e0e2e3", "e1e2e3",
            "e0e1e2e3"
        };
        for (size_t i = 0; i < 16; ++i) {
            if (std::abs(mv.coeffs[i]) > 1e-10) { // Avoid printing near-zero terms
                if (!first) os << " + ";
                os << mv.coeffs[i] << " " << labels[i];
                first = false;
            }
        }
        if (first) os << "0"; // If all coefficients are zero
        return os;
    }

    // String representation of the multiplication table
    static std::string to_table_string() {
        static const int width = 9;
        std::ostringstream oss;
        const char* labels[] = {
            "1", "e0", "e1", "e2", "e3",
            "e0e1", "e0e2", "e0e3", "e1e2", "e1e3", "e2e3",
            "e0e1e2", "e0e1e3", "e0e2e3", "e1e2e3",
            "e0e1e2e3"
        };

        // Table header
        oss << "Cl(1,3) Multiplication Table\n";
        oss << std::setw(width) << " "; // Space for row labels
        for (int j = 0; j < 16; ++j) {
            oss << std::setw(width) << labels[j];
        }
        oss << "\n";

        // Separator line
        oss << std::string(width + 16 * width, '-') << "\n";

        // Table body
        for (int i = 0; i < 16; ++i) {
            oss << std::setw(width - 1) << labels[i] << "|";
            for (int j = 0; j < 16; ++j) {
                auto [index, sign] = multiplication_table[i][j];
                std::string entry = (sign == 1 ? "+" : "-") + std::string(labels[index]);
                oss << std::setw(width) << entry;
            }
            oss << "\n";
        }

        return oss.str();
    }

private:
    // Multiplication table for basis elements
    static constexpr std::array<std::array<Product, 16>, 16> multiplication_table = make_multiplication_table(signature);
};

// Scalar multiplication (left side)
inline Clifford13 operator*(double scalar, const Clifford13& mv) {
    return mv * scalar;
}

} // namespace
