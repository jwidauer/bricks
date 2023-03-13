#include <doctest/doctest.h>

#include <bricks/charconv.hpp>
#include <cmath>
#include <iostream>
#include <stdexcept>

TEST_SUITE_BEGIN("[charconv]");

TEST_CASE_TEMPLATE("to_string with valid values", T, int, long, long long, unsigned, unsigned long,
                   unsigned long long)
{
  T value = 42;
  auto output = bricks::to_string(value);
  REQUIRE(output.is_value());
  CHECK(output.unwrap() == "42");

  if constexpr (std::is_signed_v<T>) {
    value = -42;
    output = bricks::to_string(value);
    REQUIRE(output.is_value());
    CHECK(output.unwrap() == "-42");
  }
}

TEST_CASE("to_string with float")
{
  auto output = bricks::to_string(1.0F / 3.0F, 10);
  REQUIRE(output.is_value());
  CHECK(output.unwrap() == "0.33333334");
}

TEST_CASE("to_string with invalid values")
{
  auto output = bricks::to_string(1.0F / 3.0F);
  REQUIRE(output.is_error());
  CHECK(output.unwrap_error() == std::errc::value_too_large);
}

TEST_CASE("to_string example")
{
  /// [to_string-example]
  auto output = bricks::to_string(42);
  REQUIRE(output.is_value());
  CHECK(output.unwrap() == "42");
  /// [to_string-example]
}

TEST_CASE_TEMPLATE("from_string with valid values", T, int, long, long long, unsigned,
                   unsigned long, unsigned long long)
{
  auto value = bricks::from_string<T>("42");
  REQUIRE(value.is_value());
  CHECK(value.unwrap() == 42);

  if constexpr (std::is_signed_v<T>) {
    value = bricks::from_string<T>("-42");
    CHECK(value.is_value());
    CHECK(value.unwrap() == -42);
  }
}

TEST_CASE("from_string example")
{
  /// [from_string-example]
  auto value = bricks::from_string<int>("42");
  REQUIRE(value.is_value());
  CHECK(value.unwrap() == 42);
  /// [from_string-example]
}

TEST_CASE("from_string with invalid values")
{
  bricks::result<int, std::errc> value{0};

  SUBCASE("double") { value = bricks::from_string<int>("42.0"); }
  SUBCASE("float") { value = bricks::from_string<int>("42.0f"); }
  SUBCASE("string") { value = bricks::from_string<int>("test"); }

  REQUIRE(value.is_error());
  CHECK(value.unwrap_error() == std::errc::invalid_argument);
}

TEST_SUITE_END();
