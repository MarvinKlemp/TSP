#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <locations.h>
#include <selection.h>

#include "../mock/RandomMock.h"

using ::testing::Return;

TEST(RouletteWheelSelection, is_construcable)
{
    RandomMock<long double> random_mock;

    TSP::RouletteWheelSelection selection{ random_mock };
}

TEST(ReverseSequenceMutation, is_selecting_parents_correctly)
{
    TSP::Chromosome c1{ { 1 } };
    c1.m_length = 100;
    TSP::Chromosome c2{ { 2 } };
    c2.m_length = 100;
    TSP::Chromosome c3{ { 3 } };
    c3.m_length = 100;
    TSP::Chromosome c4{ { 4 } };
    c4.m_length = 100;
    TSP::Chromosome c5{ { 5 } };
    c5.m_length = 100;
    auto population = std::make_unique<TSP::Population>();
    *population = { c1, c2, c3, c4, c5 };

    TSP::ParentMapping expected{ { c2, c4 } };

    RandomMock<long double> random_mock;
    EXPECT_CALL(random_mock, between(0, 1))
        .WillOnce(Return(0.35))
        .WillOnce(Return(0.75));

    TSP::RouletteWheelSelection selection{ random_mock };
    auto parent_mapping = selection.select_parents(population, 1);

    ASSERT_EQ(2, parent_mapping->at(0).first.m_tour[0]);
    ASSERT_EQ(4, parent_mapping->at(0).second.m_tour[0]);
}
