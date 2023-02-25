#include <doctest/doctest.h>

#include <bricks/reverse.hpp>
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
