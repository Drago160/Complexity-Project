#include "utils.h"

double SquareDistBetween(const Cord& from, const Cord& to) {
    return (from.x - to.x) * (from.x - to.x) + (from.y - to.y) * (from.y - to.y);
}

std::vector<size_t> GetDefaultNumerating(size_t size) {
    std::vector<size_t> default_numerating(size);
    for (size_t i = 0; i < size; ++i) {
        default_numerating[i] = i;
    }
    return default_numerating;
}