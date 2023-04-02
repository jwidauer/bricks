#include <doctest/doctest.h>

#include <bricks/ranges.hpp>
#include <numeric>
#include <vector>

TEST_SUITE_BEGIN("[reverse]");

TEST_CASE("example")
{
  /// [reverse-example]
  std::vector<int> v{1, 2, 3, 4, 5};
  for (auto e : bricks::reverse(v)) {
    INFO(e);  // prints 5, 4, 3, 2, 1
  }
  /// [reverse-example]
}

TEST_CASE("operator++()")
{
  std::vector<int> v = {1, 2, 3};
  auto reversed = bricks::reverse(v);
  auto it = reversed.begin();

  REQUIRE(it != reversed.end());
  CHECK(*it == 3);
  ++it;

  CHECK(*it == 2);
  ++it;

  CHECK(*it == 1);
  ++it;

  CHECK(it == reversed.end());
}

TEST_CASE("operator++(int)")
{
  std::vector<int> v = {1, 2, 3};
  auto reversed = bricks::reverse(v);
  auto it = reversed.begin();

  REQUIRE(it != reversed.end());
  CHECK(*it == 3);
  it++;

  CHECK(*it == 2);
  it++;

  CHECK(*it == 1);
  it++;

  CHECK(it == reversed.end());
}

TEST_CASE("works with range-based for loop")
{
  std::vector<int> v = {1, 2, 3};
  int nr = 3;
  for (auto e : bricks::reverse(v)) {
    CHECK(e == nr);
    nr--;
  }
}

TEST_CASE("works with std::accumulate")
{
  std::vector<int> v = {1, 2, 3};
  auto sum = std::accumulate(bricks::reverse(v).begin(), bricks::reverse(v).end(), 0);
  CHECK(sum == 6);
}

TEST_SUITE_END();
