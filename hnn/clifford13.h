#pragma once

#include "constants.h"
#include "clifford_common.h"
#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace hnn // https://github.com/hypernumbernet
{

// Header-only class for the Clifford algebra Cl(1,3) with signature (+,-,-,-)
class Clifford13 {
public:

    // Array to hold coefficients for the 16 basis elements
    using CoeffArray = std::array<double, 16>;

    CoeffArray coeffs; // Coefficients of the multivector

    // Default constructor: initializes all coefficients to zero
    Clifford13() : coeffs{} {}

    // Constructor from an array of coefficients
    explicit Clifford13(CoeffArray coeffs) : coeffs(coeffs) {}

    // Signature of the quadratic form: (+,-,-,-)
    static constexpr std::array<int8_t, 4> signature = {1, -1, -1, -1};

    static constexpr const char* labels[] = {
        "1", "e0", "e1", "e0e1", "e2", "e0e2", "e1e2", "e0e1e2",
        "e3", "e0e3", "e1e3", "e0e1e3", "e2e3", "e0e2e3", "e1e2e3", "e0e1e2e3"
    };

    static constexpr const uint8_t order[] = {
        0, 1, 2, 4, 8, 3, 5, 9, 6, 10, 12, 7, 11, 13, 14, 15
    };

    // Compute the grade of a basis element (number of 1s in the bit pattern)
    static constexpr int grade(size_t index) {
        int count = 0;
        for (; index; index >>= 1) count += index & 1;
        return count;
    }

    // Determine the sign for the reverse operation based on the grade
    static constexpr int reverse_sign(int grade) {
        return (grade == 2 || grade == 3) ? -1 : 1;
    }

    // Compute the reverse of the multivector
    Clifford13 reverse() const {
        CoeffArray new_coeffs;
        for (size_t i = 0; i < 16; ++i) {
            int g = grade(i);              // Calculate the grade of the basis element
            int sign = reverse_sign(g);    // Determine the sign based on the grade
            new_coeffs[i] = coeffs[i] * sign; // Apply the sign to the coefficient
        }
        return Clifford13(new_coeffs);     // Return the new multivector
    }

    // Determine the sign for the conjugation operation based on the grade
    static constexpr int conjugation_sign(int grade) {
        return (grade % 2 == 0) ? 1 : -1; // Returns 1 for even grades, -1 for odd grades
    }

    // Compute the conjugation of the multivector
    Clifford13 conjugation() const {
        CoeffArray new_coeffs;
        for (size_t i = 0; i < 16; ++i) {
            int g = grade(i);              // Calculate the grade of the basis element
            int sign = conjugation_sign(g); // Determine the sign based on the grade
            new_coeffs[i] = coeffs[i] * sign; // Apply the sign to the coefficient
        }
        return Clifford13(new_coeffs);     // Return the new multivector
    }

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
        static const int width = 10;
        std::ostringstream oss;

        // Table header
        oss << "Cl(1,3) Multiplication Table\n";
        oss << std::setw(width) << " "; // Space for row labels
        for (int j = 1; j < 16; ++j) {
            oss << std::setw(width) << labels[order[j]];
        }
        oss << "\n";

        // Separator line
        oss << std::string(width + 15 * width, '-') << "\n";

        // Table body
        for (int i = 1; i < 16; ++i) {
            int order_i = order[i];
            oss << std::setw(width - 1) << labels[order_i] << "|";
            for (int j = 1; j < 16; ++j) {
                auto [index, sign] = multiplication_table[order_i][order[j]];
                std::string entry = (sign == 1 ? "+" : "-") + std::string(labels[index]);
                oss << std::setw(width) << entry;
            }
            oss << "\n";
        }

        return oss.str();
    }

    static std::string to_table_string_tex1() {
        std::ostringstream oss;

        oss << R"(\begin{tabular}{r|rrrrrrrrrrrrrrr})" << "\n";
        for (int j = 1; j < 8; ++j) {
            oss << "&$" << replaceString(labels[order[j]], "e", "e_") << "$";
        }
        oss << R"(\\\hline)" << "\n";
        for (int i = 1; i < 16; ++i) {
            oss << "$" << replaceString(labels[order[i]], "e", "e_") << "$";
            for (int j = 1; j < 8; ++j) {
                auto [index, sign] = multiplication_table[order[i]][order[j]];
                std::string entry = (sign == 1 ? "+" : "-") + std::string(labels[index]);
                oss << "&$" << replaceString(entry.c_str(), "e", "e_") << "$";
            }
            oss << R"(\\)" << "\n";
        }
        oss << R"(\end{tabular})" << "\n";

        return oss.str();
    }

    static std::string to_table_string_tex2() {
        std::ostringstream oss;

        oss << R"(\begin{tabular}{r|rrrrrrrrrrrrrrr})" << "\n";
        for (int j = 8; j < 16; ++j) {
            oss << "&$" << replaceString(labels[order[j]], "e", "e_") << "$";
        }
        oss << R"(\\\hline)" << "\n";
        for (int i = 1; i < 16; ++i) {
            oss << "$" << replaceString(labels[order[i]], "e", "e_") << "$";
            for (int j = 8; j < 16; ++j) {
                auto [index, sign] = multiplication_table[order[i]][order[j]];
                std::string entry = (sign == 1 ? "+" : "-") + std::string(labels[index]);
                oss << "&$" << replaceString(entry.c_str(), "e", "e_") << "$";
            }
            oss << R"(\\)" << "\n";
        }
        oss << R"(\end{tabular})" << "\n";

        return oss.str();
    }

    static std::string to_table_string_tex() {
        std::ostringstream oss;

        oss << R"(\begin{tabular}{r|rrrrrrrrrrrrrrr})" << "\n";
        for (int j = 1; j < 16; ++j) {
            oss << "&$" << replaceString(labels[order[j]], "e", "") << "$";
        }
        oss << R"(\\\hline)" << "\n";
        for (int i = 1; i < 16; ++i) {
            oss << "$" << replaceString(labels[order[i]], "e", "") << "$";
            for (int j = 1; j < 16; ++j) {
                auto [index, sign] = multiplication_table[order[i]][order[j]];
                std::string entry = std::string(labels[index]);
                if (entry.length() == 1)
                    entry = replaceString(entry.c_str(), "1", "");
                entry = (sign == 1 ? "+" : "-") + replaceString(entry.c_str(), "e", "");
                oss << "&$" << entry << "$";
            }
            oss << R"(\\)" << "\n";
        }
        oss << R"(\end{tabular})" << "\n";

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
