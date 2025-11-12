#include "Random.h"

std::random_device Random::seedGenerator_;
std::mt19937 Random::randomEngine_ = std::mt19937(seedGenerator_());
std::uniform_real_distribution<float> Random::distribution_;


void Random::SetMinMax(float min,float max)
{
    distribution_ = std::uniform_real_distribution<float>(min, max);
}

float Random::Get()
{
    return distribution_(randomEngine_);
}