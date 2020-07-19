#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>

#include <crossover.h>
#include <locations.h>
#include <mutation.h>
#include <selection.h>
#include <tsp.h>

constexpr const std::array array_locations{
    TSP::Location{ "Ismaning", 48.229035, 11.686153 },
    TSP::Location{ "Berlin", 52.580911, 13.293884 },
    TSP::Location{ "Braunschweig", 52.278748, 10.524797 },
    TSP::Location{ "Bretten", 49.032767, 8.698372 },
    TSP::Location{ "Chemnitz", 50.829383, 12.914737 },
    TSP::Location{ "Düsseldorf", 51.274774, 6.794912 },
    TSP::Location{ "Essen", 51.450577, 7.008871 },
    TSP::Location{ "Eschborn", 50.136479, 8.570963 },
    TSP::Location{ "Görlitz", 51.145511, 14.970028 },
    TSP::Location{ "Hamburg", 53.557577, 9.986065 },
    TSP::Location{ "Hannover", 52.337987, 9.769706 },
    TSP::Location{ "Ingolstadt", 48.784417, 11.399106 },
    TSP::Location{ "Hürth", 50.886726, 6.913119 },
    TSP::Location{ "Lingen", 52.519154, 7.322185 },
    TSP::Location{ "Münster", 51.969304, 7.61428 },
    TSP::Location{ "Nürnberg", 49.429596, 11.017404 },
    TSP::Location{ "Passau", 48.571989, 13.453256 },
    TSP::Location{ "Schortens", 53.537779, 7.936809 },
    TSP::Location{ "St. Georgen", 48.126258, 8.325873 },
    TSP::Location{ "Leinfelden-Echterdingen", 48.694648, 9.161239 },
    TSP::Location{ "Walldorf", 49.295011, 8.649036 },
};
constexpr const TSP::Locations locations{ std::move(array_locations) };

int main(int argc, char* argv[])
{
    if (argc != 4) {
        std::cout << "tsp <population_size> <elites_size> <iterations>";
        return -1;
    }

    auto population_size = static_cast<std::size_t>(atoi(argv[1]));
    auto elites_size = static_cast<std::size_t>(atoi(argv[2]));
    auto iterations = static_cast<std::size_t>(atoi(argv[3]));

    std::random_device rd;
    std::mt19937 rng_engine(rd());
    TSP::UniformMt199937Random<std::size_t> rng_int{ rng_engine };
    TSP::UniformMt199937Random<long double> rng_float{ rng_engine };

    auto selection = TSP::RouletteWheelSelection{ rng_float };
    auto crossover = TSP::OrderedCrossover{ rng_int };
    auto mutation = TSP::ReverseSequenceMutation{ rng_int };
    auto genetic_tsp = TSP::GeneticTSP{
        locations,
        selection,
        crossover,
        mutation,
        population_size,
        elites_size
    };

    std::ofstream output_file("./genetic_tsp.csv");
    std::ostream_iterator<long double> output_iterator(output_file, ",");
    std::ostringstream vts;

    for (std::size_t i = 0; i < 100; i++) {
        std::cout << "Experiment: " << i + 1 << "\n";
        auto [best, stats] = genetic_tsp.iterate(iterations);

        // save to csv
        std::copy(std::begin(stats), std::end(stats), output_iterator);
        output_file << "\n";

        // print tour
        std::cout << "(" << best.m_length << ") ";
        std::cout << locations.m_locations[0].m_name << ",";
        for (auto location : best.m_tour) {
            std::cout << locations.m_locations[location].m_name << ",";
        }
        std::cout << locations.m_locations[0].m_name << "\n";

        genetic_tsp.reset_population();
    }

    return 0;
}
