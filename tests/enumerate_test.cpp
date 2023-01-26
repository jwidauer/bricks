#include <doctest/doctest.h>

#include <bricks/enumerate.hpp>
#include <vector>

TEST_SUITE_BEGIN("[enumerate]");

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
