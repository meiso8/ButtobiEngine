#pragma once
#include <random>

class Random
{
public:
    Random(float min = 0.0f, float max = 1.0f)
        : distribution_(min, max)
    {
    }

    float Get()
    {
        return distribution_(GetEngine());
    }

    void SetMinMax(float min, float max)
    {
        distribution_ = std::uniform_real_distribution<float>(min, max);
    }

private:
    static std::mt19937& GetEngine()
    {
        thread_local static std::mt19937 engine(std::random_device{}());
        return engine;
    }

    std::uniform_real_distribution<float> distribution_;
};
