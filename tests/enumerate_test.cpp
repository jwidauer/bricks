#include <doctest/doctest.h>

#include <bricks/enumerate.hpp>
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

TEST_CASE("enumerate")
{
  std::vector<int> v = {1, 2, 3};
  int idx = 0;
  for (auto [i, e] : bricks::enumerate(v)) {
    CHECK(i == idx);
    CHECK(e == v[i]);
    idx++;
  }
}

TEST_SUITE_END();
