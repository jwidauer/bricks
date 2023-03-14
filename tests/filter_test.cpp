#include <doctest/doctest.h>

#include <bricks/ranges.hpp>
#include <vector>

TEST_SUITE_BEGIN("[filter]");

TEST_CASE("example")
{
  /// [filter-example]
  std::vector<int> v{1, 2, 3, 4, 5};
  auto filtered = bricks::filter(v, [](int i) { return i % 2 == 0; });
  int expected = 2;
  for (auto i : filtered) {
    CHECK(i == expected);
    expected += 2;
  }
  /// [filter-example]
}

TEST_CASE("operator++()")
{
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto filtered = bricks::filter(v, [](int i) { return i % 2 == 0; });
  auto it = filtered.begin();

  REQUIRE(it != filtered.end());
  CHECK(*it == 2);
  ++it;

  CHECK(*it == 4);
  ++it;

  CHECK(it == filtered.end());
}

TEST_CASE("operator++(int)")
{
  std::vector<int> v = {1, 2, 3, 4, 5};
  auto filtered = bricks::filter(v, [](int i) { return i % 2 == 0; });
  auto it = filtered.begin();

  REQUIRE(it != filtered.end());
  CHECK(*it == 2);
  it++;

  CHECK(*it == 4);
  it++;

  CHECK(it == filtered.end());
}

TEST_CASE("works with empty range")
{
  std::vector<int> v = {};
  auto filtered = bricks::filter(v, [](int i) { return i % 2 == 0; });
  auto it = filtered.begin();

  CHECK(it == filtered.end());
}

TEST_CASE("works with empty predicate")
{
  std::vector<int> v = {1, 2, 3};
  auto filtered = bricks::filter(v, [](int) { return false; });
  auto it = filtered.begin();

  CHECK(it == filtered.end());
}

TEST_CASE("works with range-based for")
{
  std::vector<int> v = {1, 2, 3};
  auto filtered = bricks::filter(v, [](int i) { return i % 2 == 0; });
  for (auto i : filtered) {
    CHECK(i == 2);
  }
}

TEST_SUITE_END();
