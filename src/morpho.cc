#include "morpho.hh"

#include <cmath>

Matrix2D<int> morphoCircle(int radius) {
    int size = 2 * radius + 1;
    auto result = Matrix2D<int>(size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (std::pow(i - radius, 2) + std::pow(j - radius, 2) <= std::pow(radius, 2))
                result[i][j] = 1;
        }
    }
    return result;
}