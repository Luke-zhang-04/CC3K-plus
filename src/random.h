#ifndef RANDOM_WRAPPER_H
#define RANDOM_WRAPPER_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <random>
#include <utility>
#include <vector>

using std::size_t;

extern std::default_random_engine randomEngine;

/** @brief shuffle vector in-place */
template<typename T>
void shuffle(std::vector<T>& vec) {
    std::shuffle(vec.begin(), vec.end(), randomEngine);
}

/** @brief shuffle array in-place */
template<typename T, size_t size>
void shuffle(std::array<T, size>& arr) {
    std::shuffle(arr.begin(), arr.end(), randomEngine);
}

/** @brief random integer between [start, end] */
size_t randInt(size_t start, size_t end);

/** @brief random pair of distinct integers between [start, end] (i.e first != second) */
std::pair<size_t, size_t> randIntPair(size_t start, size_t end);

#endif
