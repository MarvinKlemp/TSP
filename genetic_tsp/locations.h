#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <random>
#include <utility>
#include <vector>

namespace TSP {
struct Location {
    using Coordinate = long double;

    const char* const m_name;
    const Coordinate m_longitude;
    const Coordinate m_latitude;

    constexpr Location(const char* const name, Coordinate longitude, Coordinate latitude)
        : m_name(name)
        , m_longitude(longitude)
        , m_latitude(latitude)
    {
    }
};

template <std::size_t Size>
struct Locations {
public:
    constexpr explicit Locations(const std::array<Location, Size>&& locations)
        : m_locations(std::move(locations))
        , m_distance_matrix({})
    {
        for (std::size_t i = 0; i < Size; i++) {
            for (std::size_t j = 0; j < Size; j++) {
                m_distance_matrix[i][j] = std::sqrt(
                    std::pow(m_locations[i].m_longitude - m_locations[j].m_longitude, 2) + std::pow(m_locations[i].m_latitude - m_locations[j].m_latitude, 2));
            }
        }
    }
    std::array<Location, Size> m_locations;
    std::array<std::array<long double, Size>, Size> m_distance_matrix;
};

// c++17 CTAD
template <std::size_t Size>
Locations(const std::array<Location, Size>&& locations)->Locations<Size>;

using Tour = std::vector<std::size_t>;
using Edge = std::pair<std::size_t, std::size_t>;

struct Chromosome {
    Chromosome()
        : m_tour({})
        , m_length(0U)
    {
    }
    Chromosome(const Chromosome& other) = default;
    explicit Chromosome(std::size_t size)
        : m_tour((size))
        , m_length(0U)
    {
    }
    explicit Chromosome(Tour&& tour)
        : m_tour(std::move(tour))
        , m_length(0U)
    {
    }
    static Chromosome random(std::size_t size)
    {
        // start and beginning are not stored as they are always locations[0]
        std::vector<std::size_t> tour(size - 1);
        for (std::size_t i = 1; i < size; i++) {
            tour.at(i - 1) = i;
        }

        // randomize the tour
        std::shuffle(
            std::begin(tour),
            std::end(tour),
            std::mt19937(std::random_device()()));
        return Chromosome{ std::move(tour) };
    }

    template <std::size_t Size>
    void evaluate(const Locations<Size>& locations)
    {
        // start and beginning are not stored as they are always locations[0]

        // distance: start to first element
        long double length{ locations.m_distance_matrix[0][m_tour[0]] };
        for (std::size_t i = 0; i < m_tour.size() - 1; i++) {
            length += locations.m_distance_matrix[m_tour[i]][m_tour[i + 1]];
        }
        // distance: last element to first element
        length += locations.m_distance_matrix[m_tour[m_tour.size() - 1]][0];

        m_length = length;
    }

    long double fitness() const
    {
        // inversion is needed as we have a minimization problem
        return 1.f / m_length;
    }

    std::string to_string() const
    {
        return std::string(begin(m_tour), end(m_tour));
    }

    Tour m_tour;
    long double m_length;
};

using Population = std::vector<Chromosome>;
using Parents = std::pair<Chromosome, Chromosome>;
using ParentMapping = std::vector<Parents>;
}
