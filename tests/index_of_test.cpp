#include <doctest/doctest.h>

#include <bricks/algorithm.hpp>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_set>

#include "string_makers.hpp"

TEST_SUITE_BEGIN("[algorithm]");

TEST_CASE("index_of")
{
  SUBCASE("example")
  {
    /// [index_of-example]
    std::vector<int> vec = {1, 2, 3, 4, 5};
    INFO(bricks::index_of(vec, 3));  // prints 2
    /// [index_of-example]
  }
  SUBCASE("vector")
  {
    std::vector<int> v = {1, 2, 3};
    CHECK(bricks::index_of(v, 1) == 0);
    CHECK(bricks::index_of(v, 2) == 1);
    CHECK(bricks::index_of(v, 3) == 2);
    CHECK(bricks::index_of(v, 4) == std::nullopt);
  }

  SUBCASE("list")
  {
    std::list<int> l = {1, 2, 3};
    CHECK(bricks::index_of(l, 1) == 0);
    CHECK(bricks::index_of(l, 2) == 1);
    CHECK(bricks::index_of(l, 3) == 2);
    CHECK(bricks::index_of(l, 4) == std::nullopt);
  }

  SUBCASE("map")
  {
    std::map<int, int> m = {{1, 2}, {3, 4}, {5, 6}};
    CHECK(bricks::index_of(m, 1) == 0);
    CHECK(bricks::index_of(m, 3) == 1);
    CHECK(bricks::index_of(m, 5) == 2);
    CHECK(bricks::index_of(m, 2) == std::nullopt);
    CHECK(bricks::index_of(m, 4) == std::nullopt);
    CHECK(bricks::index_of(m, 6) == std::nullopt);
  }

  SUBCASE("set")
  {
    std::set<int> s = {1, 2, 3};
    CHECK(bricks::index_of(s, 1) == 0);
    CHECK(bricks::index_of(s, 2) == 1);
    CHECK(bricks::index_of(s, 3) == 2);
    CHECK(bricks::index_of(s, 4) == std::nullopt);
  }

  SUBCASE("string")
  {
    std::string s = "abc";
    CHECK(bricks::index_of(s, 'a') == 0);
    CHECK(bricks::index_of(s, 'b') == 1);
    CHECK(bricks::index_of(s, 'c') == 2);
    CHECK(bricks::index_of(s, 'd') == std::nullopt);
  }
}

TEST_CASE("index_of_if")
{
  SUBCASE("example")
  {
    /// [index_of_if-example]
    std::vector<int> vec = {1, 2, 3, 4, 5};
    INFO(bricks::index_of_if(vec, [](int i) { return i % 2 == 0; }));  // prints 1
    /// [index_of_if-example]
  }

  SUBCASE("test")
  {
    std::vector<int> v = {1, 2, 3};
    CHECK(bricks::index_of_if(v, [](int i) { return i == 1; }) == 0);
    CHECK(bricks::index_of_if(v, [](int i) { return i == 2; }) == 1);
    CHECK(bricks::index_of_if(v, [](int i) { return i == 3; }) == 2);
    CHECK(bricks::index_of_if(v, [](int i) { return i == 4; }) == std::nullopt);
    CHECK_THROWS_AS(bricks::index_of_if(v,
                                        [](int i) {
                                          throw std::logic_error("test exception");
                                          return i == 1;
                                        }),
                    std::logic_error);
  }
}

TEST_SUITE_END();
