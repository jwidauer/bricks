#include <doctest/doctest.h>

#include <bricks/enumerate.hpp>
#include <numeric>
#include <tuple>
#include <vector>

TEST_SUITE_BEGIN("[enumerate]");

TEST_CASE("example")
{
  /// [enumerate-example]
  std::vector<int> v{1, 2, 3, 4, 5};
  for (auto [i, e] : bricks::enumerate(v)) {
    auto idx = i;
    auto elem = e;
    INFO(idx, ": ", elem);  // prints 0: 1, 1: 2, 2: 3, 3: 4, 4: 5
  }
  /// [enumerate-example]
}

TEST_CASE("operator++()")
{
  std::vector<int> v = {1, 2, 3};
  auto enumerated = bricks::enumerate(v);
  auto it = enumerated.begin();

  REQUIRE(it != enumerated.end());
  auto [first, second] = *it;
  CHECK(first == 0);
  CHECK(second == 1);
  ++it;

  std::tie(first, second) = *it;
  CHECK(first == 1);
  CHECK(second == 2);
  ++it;

  std::tie(first, second) = *it;
  CHECK(first == 2);
  CHECK(second == 3);
  ++it;

  CHECK(it == enumerated.end());
}

TEST_CASE("operator++(int)")
{
  std::vector<int> v = {1, 2, 3};
  auto enumerated = bricks::enumerate(v);
  auto it = enumerated.begin();

  REQUIRE(it != enumerated.end());
  auto [first, second] = *it;
  CHECK(first == 0);
  CHECK(second == 1);
  it++;

  std::tie(first, second) = *it;
  CHECK(first == 1);
  CHECK(second == 2);
  it++;

  std::tie(first, second) = *it;
  CHECK(first == 2);
  CHECK(second == 3);
  it++;

  CHECK(it == enumerated.end());
}

TEST_CASE("works with range based for loop")
{
  std::vector<int> v = {1, 2, 3};
  int idx = 0;
  for (auto [i, e] : bricks::enumerate(v)) {
    CHECK(i == idx);
    CHECK(e == v[i]);
    idx++;
  }
}

TEST_CASE("works with algorithm")
{
  std::vector<int> v = {1, 2, 3};
  auto enumerated = bricks::enumerate(v);
  auto sum = std::accumulate(enumerated.begin(), enumerated.end(), 0, [](int acc, auto pair) {
    auto [idx, elem] = pair;
    return acc + idx + elem;
  });
  CHECK(sum == 9);
}

TEST_SUITE_END();
