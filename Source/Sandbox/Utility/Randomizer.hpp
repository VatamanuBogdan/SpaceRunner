#pragma once

#include <random>
#include <vector>
#include <iterator>
#include <initializer_list>

class Randomizer {
public:
    Randomizer() noexcept
    : mt(std::random_device()()), dist(0.0f, 1.0f) {
    }

    float operator()() { return dist(mt); }

    float Range(float start, float end) {
        return (end - start) * this->operator()() + start;
    }

private:
    std::mt19937 mt;
    std::uniform_real_distribution<float> dist;
};
