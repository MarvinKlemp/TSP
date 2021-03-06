#pragma once

#include <algorithm>
#include <memory>
#include <random>

#include <locations.h>
#include <random.h>

namespace TSP {
class Crossover {
public:
    virtual ~Crossover() = default;
    std::unique_ptr<Population> crossover_population(std::unique_ptr<ParentMapping>&& parent_mapping) const
    {
        auto new_population = std::make_unique<Population>();

        for (auto& parents : *parent_mapping) {
            crossover(parents, new_population);
        }

        return new_population;
    }
    virtual void crossover(Parents& parents, std::unique_ptr<Population>& population) const = 0;
};

class OrderedCrossover : public Crossover {
public:
    explicit OrderedCrossover(Random<std::size_t>& rng)
        : m_rng(rng)
    {
    }

    // in ordered crossover a sequence from the first parent is maintained
    // each location from the other child will be added to this maintained sequence
    void crossover(Parents& parents, std::unique_ptr<Population>& population) const override
    {
        // Size is the same for all children
        std::size_t size = parents.first.m_tour.size();

        // determine crossover points
        auto [start, end] = m_rng.range_between(0, size - 1);

        auto child = Chromosome{};
        std::copy(
            std::begin(parents.first.m_tour) + start,
            std::begin(parents.first.m_tour) + end,
            back_inserter(child.m_tour));

        // find locations that are not in the maintained sequence and add them
        for (std::size_t i = 0; i < size; i++) {
            auto gene_second_parent = parents.second.m_tour[i];

            auto it = std::find(
                std::begin(child.m_tour),
                std::end(child.m_tour),
                gene_second_parent);
            if (it == std::end(child.m_tour)) {
                child.m_tour.push_back(gene_second_parent);
            }
        }

        // rotate (sequence should be at the correct place)
        std::rotate(
            std::rbegin(child.m_tour),
            std::rbegin(child.m_tour) + start,
            std::rend(child.m_tour));

        population->push_back(child);
    }

private:
    Random<std::size_t>& m_rng;
};
}
