#pragma once

#include <utility>

namespace TSP {
template <typename T>
class Random {
public:
    virtual ~Random() = default;

    // inclusive random number between min and max
    virtual T between(T min, T max) = 0;

    // inclusive random number between min and max
    // returns a pair with the start as first number and end as second number
    virtual std::pair<T, T> range_between(T min, T max) = 0;
};

template <typename T>
class UniformMt199937Random : public Random<T> {
};

template <>
class UniformMt199937Random<std::size_t> : public Random<std::size_t> {
public:
    explicit UniformMt199937Random(std::mt19937& rng_engine)
        : m_rng_engine(rng_engine)
    {
    }
    std::size_t between(std::size_t min, std::size_t max) override
    {
        std::uniform_int_distribution<std::size_t> uni(min, max);

        return uni(m_rng_engine);
    }

    std::pair<std::size_t, std::size_t> range_between(std::size_t min, std::size_t max) override
    {
        std::uniform_int_distribution<std::size_t> uni(min, max);

        std::size_t a = uni(m_rng_engine);
        std::size_t b = uni(m_rng_engine);
        std::size_t start = std::min(a, b);
        std::size_t end = std::max(a, b);

        return { start, end };
    }

private:
    std::mt19937& m_rng_engine;
};

template <>
class UniformMt199937Random<long double> : public Random<long double> {
public:
    explicit UniformMt199937Random(std::mt19937& rng_engine)
        : m_rng_engine(rng_engine)
    {
    }
    long double between(long double min, long double max) override
    {
        std::uniform_real_distribution<long double> uni(min, max);

        return uni(m_rng_engine);
    }

    std::pair<long double, long double> range_between(long double min, long double max) override
    {
        std::uniform_real_distribution<long double> uni(min, max);

        long double a = uni(m_rng_engine);
        long double b = uni(m_rng_engine);
        long double start = std::min(a, b);
        long double end = std::max(a, b);

        return { start, end };
    }

private:
    std::mt19937& m_rng_engine;
};
}
