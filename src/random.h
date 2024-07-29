#ifndef RANDOM_H
#define RANDOM_H

#include <array>
#include <random>
#include <vector>

extern std::default_random_engine randomEngine;

/** @brief shuffle vector in-place */
template<typename T>
void shuffle(std::vector<T>&);

/** @brief shuffle array in-place */
template<typename T, size_t size>
void shuffle(std::array<T, size>&);

/** @brief random integer between [start, end] */
int randInt(int start, int end);

#endif
