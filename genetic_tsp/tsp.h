#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <crossover.h>
#include <mutation.h>
#include <selection.h>

namespace TSP {
template <std::size_t Locations_Size>
class GeneticTSP {
public:
    using Statistics = std::vector<long double>;
    GeneticTSP(
        const Locations<Locations_Size>& locations,
        const Selection& selection,
        const Crossover& crossover,
        const Mutation& mutation,
        std::size_t population_size,
        std::size_t elite_size)
        : m_population(std::make_unique<Population>())
        , m_locations(locations)
        , m_selection(selection)
        , m_crossover(crossover)
        , m_mutation(mutation)
        , m_population_size(population_size)
        , m_elite_size(elite_size)
    {
        reset_population();
    }

    void reset_population()
    {
        m_population = std::make_unique<Population>();
        for (std::size_t i = 0; i < m_population_size; i++) {
            m_population->push_back(Chromosome::random(Locations_Size - 1));
        }
        evaluate(m_population);
        sort_by_fitness(m_population);
    }
    std::pair<Chromosome, Statistics> iterate(std::size_t iterations)
    {
        Statistics stats{};
        for (std::size_t i = 0; i < iterations; i++) {
            // save elites to add them later
            std::vector<Chromosome> elites{};
            elites.reserve(m_elite_size);
            std::copy(std::begin(*m_population), std::begin(*m_population) + m_elite_size, std::back_inserter(elites));

            // genetic algorithm steps
            auto parent_selection = m_selection.select_parents(m_population, m_population_size - m_elite_size);
            auto children_population = m_crossover.crossover_population(std::move(parent_selection));
            m_mutation.mutate_population(children_population);

            // add elites to the next population
            children_population->insert(std::end(*children_population), std::begin(elites), std::end(elites));

            m_population = std::move(children_population);
            evaluate(m_population);
            sort_by_fitness(m_population);

            // save statistics
            stats.push_back(m_population->at(0).m_length);
        }

        return { m_population->at(0), stats };
    }
    Chromosome& best() const
    {
        return (*m_population)[0];
    }

private:
    void evaluate(std::unique_ptr<Population>& population)
    {
        for (Chromosome& chromosome : *population) {
            chromosome.evaluate(m_locations);
        }
    }
    void sort_by_fitness(std::unique_ptr<Population>& population)
    {
        std::sort(std::begin(*population), std::end(*population), [](const Chromosome& chromosome, const Chromosome& other_chromosome) {
            return chromosome.fitness() > other_chromosome.fitness();
        });
    }

    std::unique_ptr<Population> m_population;
    const Locations<Locations_Size>& m_locations;
    const Selection& m_selection;
    const Crossover& m_crossover;
    const Mutation& m_mutation;
    const std::size_t m_population_size;
    const std::size_t m_elite_size;
};

// c++17 CTAD
template <std::size_t Locations_Size>
GeneticTSP(const Locations<Locations_Size>& locations, const Selection& selection, const Crossover& crossover, const Mutation& mutation, std::size_t population_size, std::size_t elite_size)->GeneticTSP<Locations_Size>;
}
