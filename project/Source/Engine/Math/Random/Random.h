#pragma once
#include<random>

class Random
{
public:
   static void SetMinMax(const float min,const float max);
   static float Get();
private:
    static std::random_device seedGenerator_;
    static std::mt19937 randomEngine_;
    static std::uniform_real_distribution<float> distribution_;
};

