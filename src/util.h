#ifndef UTIL_H
#define UTIL_H

#include <array>
#include <cstddef>
#include <utility>
#include <vector>

using std::size_t;

/**
 * @brief Create a Vector object from pairs in `elements`, where the `first` is the number of items
 * and `second` is the value
 */
template<typename T, size_t size>
std::vector<T> createVector(const std::array<std::pair<size_t, T>, size> elements) {
    std::vector<T> vec;

    for (const auto& elem : elements) {
        vec.insert(vec.end(), elem.first, elem.second);
    }

    return vec;
}

#endif
