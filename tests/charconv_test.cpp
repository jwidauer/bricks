#include <doctest/doctest.h>

#include <bricks/charconv.hpp>
#include <cmath>
#include <stdexcept>

TEST_SUITE_BEGIN("[charconv]");

TEST_CASE_TEMPLATE("non throwing to_string", T, int, long, long long, unsigned, unsigned long,
                   unsigned long long)
{
  T value = 42;
  std::string output;
  REQUIRE(bricks::to_string(value, output));
  CHECK(output == "42");

  if constexpr (std::is_signed_v<T>) {
    value = -42;
    REQUIRE(bricks::to_string(value, output));
    CHECK(output == "-42");
  }
}

TEST_CASE_TEMPLATE("from_string with valid values", T, int, long, long long, unsigned,
                   unsigned long, unsigned long long)
{
  T value;
  CHECK(bricks::from_string("42", value));
  CHECK(value == 42);

  if constexpr (std::is_signed_v<T>) {
    CHECK(bricks::from_string("-42", value));
    CHECK(value == -42);
  }
}

TEST_CASE("from_string with invalid values")
{
  int value;
  CHECK_FALSE(bricks::from_string("42.0", value));
  CHECK_FALSE(bricks::from_string("42.0f", value));
  CHECK_FALSE(bricks::from_string("test", value));
}

TEST_SUITE_END();
