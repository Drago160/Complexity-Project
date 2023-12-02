#pragma once

#include "random.h"
#include <random>

template <typename RandomGen>
inline int RandomInt(int left_bound, int right_bound, RandomGen& gen) {
    std::uniform_int_distribution<int> distribution(left_bound, right_bound);
    return distribution(gen);
}

template <typename RandomGen>
inline bool RandomBool(RandomGen& gen) {
    return RandomInt(0, 1, gen) == 0 ? false : true;
}

template <typename RandomGen>
size_t GetRandomNotUsed(const std::vector<bool>& used, size_t used_number, RandomGen& gen) {
    size_t rand_idx = RandomInt(0, used.size() - used_number - 1, gen);
    for (size_t i = 0; i < used.size(); ++i) {
        if (used[i]) {
            continue;
        }
        if (rand_idx == 0) {
            return i;
        }
        --rand_idx;
    }
    if (rand_idx == 0) {
        return used.size() - 1;
    }
    return used.size();
}

template <typename RandomGen>
std::vector<size_t> RandomDiffNumbers(size_t size, size_t max_num, RandomGen& gen) {
    static std::vector<bool> used(max_num);
    used.reserve(max_num);
    std::fill(used.begin(), used.end(), false);

    std::vector<size_t> ret;
    ret.reserve(size);

    while (ret.size() != size) {
        auto rand_num = RandomInt(0, max_num, gen);
        if (used[rand_num]) {
            continue;
        }
        used[rand_num] = true;
        ret.push_back(rand_num);
    }
    return ret;
}
