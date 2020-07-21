#pragma once

#include <algorithm>
#include <execution>
#include <memory>

#include <locations.h>
#include <random.h>

namespace TSP {
class Mutation {
public:
    virtual ~Mutation() = default;
    void mutate_population(std::unique_ptr<Population>& population) const
    {
        std::for_each(
            std::execution::par_unseq,
            std::begin(*population),
            std::end(*population),
            [this](auto& chromosome) {
                mutate(chromosome);
            });
    }
    virtual void mutate(Chromosome& chromosome) const = 0;
};

class ReverseSequenceMutation : public Mutation {
public:
    explicit ReverseSequenceMutation(Random<std::size_t>& rng)
        : m_rng(rng)
    {
    }
    // in reverse sequence mutation a random sequence is reversed
    void mutate(Chromosome& chromosome) const override
    {
        if (m_rng.between(0, 9) != 0) {
            return;
        }

        // determine sequence
        auto [start, end] = m_rng.range_between(0, chromosome.m_tour.size() - 1);

        // reverse chromosome between mutation points
        std::reverse(
            std::begin(chromosome.m_tour) + start,
            std::begin(chromosome.m_tour) + end);
    }

private:
    Random<std::size_t>& m_rng;
};
}
