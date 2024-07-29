#include "random.h"

#include <array>
#include <random>
#include <unordered_set>
#include <utility>
#include <vector>

std::default_random_engine randomEngine{};

size_t randInt(size_t start, size_t end) {
    return std::uniform_int_distribution<size_t>(start, end)(randomEngine);
}

std::pair<size_t, size_t> randIntPair(size_t start, size_t end) {
    std::pair<size_t, size_t> pair{randInt(start, end), randInt(start, end - 1)};

    if (pair.second >= pair.first) {
        pair.second++;
    }

    return pair;
}
