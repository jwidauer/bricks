#include <doctest/doctest.h>

#include <bricks/reverse.hpp>
#include <vector>

TEST_SUITE_BEGIN("[reverse]");

TEST_CASE("reverse")
{
  std::vector<int> v = {1, 2, 3};
  int nr = 3;
  for (auto e : bricks::reverse(v)) {
    CHECK(e == nr);
    nr--;
  }
}

TEST_SUITE_END();
