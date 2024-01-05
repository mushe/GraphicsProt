#pragma once

#include "Core/Common.h"
#include <vector>
#include <iostream>
#include <random>

class Random
{
public:
    static int Range(int min, int max)
    {
        return getRD()() % (max - min + 1) + min;
    }

    static float Range(float min, float max)
    {
        std::uniform_real_distribution<float> dis(min, max);
        return dis(getRE());
    }
    static glm::vec3 RandomVector()
    {
		return glm::vec3(Range(-1.0f, 1.0f), Range(-1.0f, 1.0f), Range(-1.0f, 1.0f));
	}

private:
    static std::random_device& getRD()
    {
        static std::random_device rd;
        return rd;
    }

    static std::mt19937& getRE()
    {
        static std::mt19937 gen(getRD()());
        return gen;
    }
};