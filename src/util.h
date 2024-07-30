#ifndef UTIL_H
#define UTIL_H

#include <array>
#include <cstddef>
#include <utility>
#include <vector>

/**
 * @brief Create a Vector object from pairs in `elements`, where the `first` is the number of items
 * and `second` is the value
 */
template<typename T, std::size_t size>
std::vector<T> createVector(const std::array<std::pair<std::size_t, T>, size> elements) {
    std::vector<T> vec;

    for (const auto& elem : elements) {
        vec.insert(vec.end(), elem.first, elem.second);
    }

    return vec;
}

#endif
