#pragma once

#include "Core/Common.h"
#include <vector>
#include <algorithm>
#include <random>

class Algorithm
{
public:

    template <typename T>
    static void Shuffle(std::vector<T> &v)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(v.begin(), v.end(), g);
    }
};