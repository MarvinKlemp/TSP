#pragma once

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <random.h>

template <typename T>
class RandomMock : public TSP::Random<T> {
public:
    MOCK_METHOD2_T(between, T(T min, T max));
    MOCK_METHOD2_T(range_between, std::pair<T, T>(T min, T max));
};
