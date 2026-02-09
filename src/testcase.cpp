#include "catch.hpp"

// Forward declaration of student function
int add(int a, int b);

TEST_CASE("Addition of two numbers", "[add]") {
    REQUIRE(add(1, 2) == 3);
    REQUIRE(add(0, 0) == 0);
    REQUIRE(add(-1, 1) == 0);
    REQUIRE(add(100, 200) == 300);
}

// You can add more test cases here for different functions
