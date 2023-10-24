#include <doctest/doctest.h>

#include <bricks/algorithm.hpp>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "string_makers.hpp"

TEST_SUITE_BEGIN("[algorithm]");

TEST_CASE("find")
{
  SUBCASE("vector")
  {
    std::vector<int> v = {1, 2, 3, 4, 5};
    CHECK(bricks::find(v, 3) == 3);
    CHECK(bricks::find(v, 6) == std::nullopt);
  }

  SUBCASE("set")
  {
    std::set<int> s = {1, 2, 3, 4, 5};
    CHECK(bricks::find(s, 3) == 3);
    CHECK(bricks::find(s, 6) == std::nullopt);
  }

  SUBCASE("map")
  {
    SUBCASE("with different key, value types")
    {
      std::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
      CHECK(bricks::find(m, 2) == "two");
      CHECK(bricks::find(m, 4) == std::nullopt);
    }

    SUBCASE("with same key, value types")
    {
      std::map<int, int> m = {{1, 2}, {2, 3}, {3, 4}};
      CHECK(bricks::find(m, 2) == 3);
      CHECK(bricks::find(m, 4) == std::nullopt);
    }
  }

  SUBCASE("string")
  {
    std::string s = "hello";
    CHECK(bricks::find(s, 'e') == 'e');
    CHECK(bricks::find(s, 'x') == std::nullopt);
  }

  SUBCASE("string_view")
  {
    std::string_view s = "hello";
    CHECK(bricks::find(s, 'e') == 'e');
    CHECK(bricks::find(s, 'x') == std::nullopt);
  }

  SUBCASE("example")
  {
    /// [find-example]
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto index = bricks::find(v, 3);
    if (index) {
      INFO(*index);  // prints 2
    }
    /// [find-example]
  }
}

TEST_SUITE_END();
