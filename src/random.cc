#include "random.h"

std::default_random_engine randomEngine{};

template<typename T>
void shuffle(std::vector<T>& vec) {
    std::shuffle(vec.begin(), vec.end(), randomEngine);
}

int randInt(int start, int end) {
    return std::uniform_int_distribution<int>(start, end + 1)(randomEngine);
}
