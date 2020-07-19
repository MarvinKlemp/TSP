#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <locations.h>
#include <mutation.h>

#include "../mock/RandomMock.h"

using ::testing::Return;

TEST(ReverseSequenceMutation, is_construcable)
{
    RandomMock<std::size_t> random_mock;

    TSP::ReverseSequenceMutation m{ random_mock };
}

TEST(ReverseSequenceMutation, is_only_mutating_every_10th_chromosome)
{
    TSP::Chromosome c{};
    RandomMock<std::size_t> random_mock;
    EXPECT_CALL(random_mock, between(0, 9)).WillOnce(Return(1));
    EXPECT_CALL(random_mock, range_between(0, 0)).Times(0);

    TSP::ReverseSequenceMutation m{ random_mock };

    m.mutate(c);
}

TEST(ReverseSequenceMutation, is_reversing_the_sequence_correctly)
{
    auto expected = std::vector<std::size_t>{ 0, 1, 2, 5, 4, 3, 6, 7, 8, 9 };

    TSP::Chromosome c{};
    c.m_tour = std::vector<std::size_t>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    RandomMock<std::size_t> random_mock;
    EXPECT_CALL(random_mock, between(0, 9)).WillOnce(Return(0));
    EXPECT_CALL(random_mock, range_between(0, 9)).WillOnce(Return(std::pair{ 3, 6 }));

    TSP::ReverseSequenceMutation m{ random_mock };
    m.mutate(c);

    ASSERT_EQ(expected, c.m_tour);
}
