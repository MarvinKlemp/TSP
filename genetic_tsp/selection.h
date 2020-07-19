#pragma once

#include <memory>
#include <numeric>
#include <utility>
#include <vector>

#include <locations.h>
#include <random.h>

namespace TSP {
class Selection {
public:
    virtual ~Selection() = default;
    virtual std::unique_ptr<ParentMapping> select_parents(std::unique_ptr<Population>& population, std::size_t num) const = 0;
};

class RouletteWheelSelection : public Selection {
public:
    explicit RouletteWheelSelection(Random<long double>& rng)
        : m_rng(rng)
    {
    }

    // in roulette wheel selection a random chromosome from the population is picked as parent
    // the chance of being picked is related to the fitness of the chromosome (good fitness = picked often)
    std::unique_ptr<ParentMapping> select_parents(std::unique_ptr<Population>& population, std::size_t num) const override
    {
        // calculate total fitness as normalization factor
        auto total_fitness = std::accumulate(
            std::begin(*population),
            std::end(*population),
            static_cast<long double>(0),
            [](long double acc, const Chromosome& chromosome) {
                return acc + chromosome.fitness();
            });

        auto parents = std::make_unique<ParentMapping>();
        for (std::size_t i = 0; i < num; i++) {
            auto child = std::pair<Chromosome, Chromosome>{
                pick(*population, total_fitness),
                pick(*population, total_fitness)
            };
            parents->push_back(std::move(child));
        }

        return parents;
    }

private:
    Chromosome& pick(Population& population, long double total_fitness) const
    {
        auto rng = m_rng.between(0, 1);

        // iterate the chromosomes in population and add their probability factor
        // until one chromosome is bigger than the random
        long double offset = 0;
        for (auto& chromosome : population) {
            auto probability = (1.f / static_cast<long double>(population.size() - 1)) * (1 - (chromosome.fitness() / total_fitness));
            offset += probability;

            if (rng < offset) {
                return chromosome;
            }
        }

        return population[0];
    }

    Random<long double>& m_rng;
};
}
