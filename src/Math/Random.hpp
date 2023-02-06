#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

namespace Random {
    static std::random_device rd;
    static std::mt19937 engine(rd());

    static float getNormalizedFloat() {
        return (engine() % 1000) / 1000.0f;
    }     
}

#endif