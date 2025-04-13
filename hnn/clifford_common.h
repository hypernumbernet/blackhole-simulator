#pragma once

#include <array>
#include <cstdint>

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

} // namespace
