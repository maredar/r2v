#include <gtest/gtest.h>
#include "point2.hpp"

using namespace r2v;

TEST(point2test, distance_result) {
    for(int i = 0; i < 100; ++i) {
        point2<uint32_t> from{ static_cast<uint32_t>(std::rand()), static_cast<uint32_t>(std::rand()) };
        point2<uint32_t> to{ static_cast<uint32_t>(std::rand()), static_cast<uint32_t>(std::rand())  };
        auto x_diff = from.x > to.x ? from.x - to.x : to.x - from.x;
        auto y_diff = from.y > to.y ? from.y - to.y : to.y - from.y;
        auto dist = std::sqrt(std::pow(x_diff, 2) + std::pow(y_diff, 2));

        ASSERT_EQ(distance(from, to), dist);
        ASSERT_EQ(distance(to, from), dist);
    }
    point2<uint32_t> from{ 0, 0 };
    point2<uint32_t> to{ 0, 0 };
    ASSERT_EQ(distance(from, to), 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}