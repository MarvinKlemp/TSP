#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <crossover.h>
#include <locations.h>

#include "../mock/RandomMock.h"

using ::testing::Return;

TEST(OrderedCrossover, is_construcable)
{
    RandomMock<std::size_t> random_mock;

    TSP::OrderedCrossover crossover{ random_mock };
}

TEST(OrderedCrossover, is_performing_the_crossover_correctly)
{
    auto expected = std::vector<std::size_t>{ 2, 1, 0, 3, 4, 5, 9, 8, 7, 6 };

    TSP::Chromosome p1{ {} };
    p1.m_tour = std::vector<std::size_t>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    TSP::Chromosome p2{ {} };
    p2.m_tour = std::vector<std::size_t>{ 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

    RandomMock<std::size_t> random_mock;
    EXPECT_CALL(random_mock, range_between(0, 9)).WillOnce(Return(std::pair{ 3, 6 }));

    TSP::OrderedCrossover crossover{ random_mock };

    auto population = std::make_unique<TSP::Population>();
    auto parents = std::pair{ p1, p2 };
    crossover.crossover(parents, population);

    ASSERT_EQ(expected, population->at(0).m_tour);
}