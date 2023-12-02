#pragma once
#include <vector>

struct Cord {
    double x;
    double y;
};

double SquareDistBetween(const Cord& from, const Cord& to);

std::vector<size_t> GetDefaultNumerating(size_t size);

template <typename ForwardIterator, typename Comp>
inline void BubbleSort(ForwardIterator begin, ForwardIterator end, Comp comp) {
    for (ForwardIterator sorted = begin; begin != end; end = sorted) {
        sorted = begin;
        for (ForwardIterator current = begin, prev = begin; ++current != end; ++prev) {
            if (comp(*current, *prev)) {
                std::iter_swap(current, prev);
                sorted = current;
            }
        }
    }
}