#pragma once
#include <random>
#include<concepts>
#include <type_traits>

template<typename T>
concept UniformIntType =
std::same_as<std::remove_cv_t<T>, short> ||
std::same_as<std::remove_cv_t<T>, int> ||
std::same_as<std::remove_cv_t<T>, long> ||
std::same_as<std::remove_cv_t<T>, long long> ||
std::same_as<std::remove_cv_t<T>, unsigned short> ||
std::same_as<std::remove_cv_t<T>, unsigned int> ||
std::same_as<std::remove_cv_t<T>, unsigned long> ||
std::same_as<std::remove_cv_t<T>, unsigned long long>;

//受け取る型を指定することができる。
template<typename T>
concept Number = UniformIntType<T> ||std::floating_point<T>;

template<Number T>
class Random
{
private:
    //std::conditional_t<条件, 真の時の型, 偽の時の型>
    using Distribution = std::conditional_t<
        UniformIntType<T>,
        std::uniform_int_distribution<T>,
        std::uniform_real_distribution<T>
    >;
    Distribution distribution_;

    static std::mt19937& GetEngine()
    {
        thread_local static std::mt19937 engine(std::random_device{}());
        return engine;
    }
public:
    Random(T min = static_cast<T>(0), T max = static_cast<T>(1))
        : distribution_(min, max)
    {
    }

    T Get()
    {
        return distribution_(GetEngine());
    }

    void SetMinMax(T min, T max) {
        distribution_ = Distribution(min, max);
    }

};
