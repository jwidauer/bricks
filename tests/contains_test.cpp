#include <doctest/doctest.h>

#include <bricks/algorithm.hpp>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

TEST_SUITE_BEGIN("[algorithm]");

TEST_CASE("contains")
{
  SUBCASE("example")
  {
    /// [contains-example]
    std::vector<int> vec = {1, 2, 3, 4, 5};
    INFO(bricks::contains(vec, 3));  // prints true
    INFO(bricks::contains(vec, 6));  // prints false
    /// [contains-example]
  }

  SUBCASE("vector")
  {
    std::vector<int> v = {1, 2, 3};
    CHECK(bricks::contains(v, 1));
    CHECK(bricks::contains(v, 2));
    CHECK(bricks::contains(v, 3));
    CHECK_FALSE(bricks::contains(v, 4));
  }

  SUBCASE("map")
  {
    std::map<int, int> m = {{1, 2}, {3, 4}, {5, 6}};
    CHECK(bricks::contains(m, 1));
    CHECK(bricks::contains(m, 3));
    CHECK(bricks::contains(m, 5));
    CHECK_FALSE(bricks::contains(m, 2));
    CHECK_FALSE(bricks::contains(m, 4));
    CHECK_FALSE(bricks::contains(m, 6));
  }

  SUBCASE("set")
  {
    std::set<int> s = {1, 2, 3};
    CHECK(bricks::contains(s, 1));
    CHECK(bricks::contains(s, 2));
    CHECK(bricks::contains(s, 3));
    CHECK_FALSE(bricks::contains(s, 4));
  }

  SUBCASE("unordered_map")
  {
    std::unordered_map<int, int> m = {{1, 2}, {3, 4}, {5, 6}};
    CHECK(bricks::contains(m, 1));
    CHECK(bricks::contains(m, 3));
    CHECK(bricks::contains(m, 5));
    CHECK_FALSE(bricks::contains(m, 2));
    CHECK_FALSE(bricks::contains(m, 4));
    CHECK_FALSE(bricks::contains(m, 6));
  }

  SUBCASE("unordered_set")
  {
    std::unordered_set<int> s = {1, 2, 3};
    CHECK(bricks::contains(s, 1));
    CHECK(bricks::contains(s, 2));
    CHECK(bricks::contains(s, 3));
    CHECK_FALSE(bricks::contains(s, 4));
  }

  SUBCASE("string")
  {
    std::string s = "abc";
    CHECK(bricks::contains(s, 'a'));
    CHECK(bricks::contains(s, 'b'));
    CHECK(bricks::contains(s, 'c'));
    CHECK_FALSE(bricks::contains(s, 'd'));
  }
}

TEST_CASE("contains_if")
{
  SUBCASE("test")
  {
    std::vector<int> v = {1, 2, 3};
    CHECK(bricks::contains_if(v, [](int i) { return i == 1; }));
    CHECK(bricks::contains_if(v, [](int i) { return i == 2; }));
    CHECK(bricks::contains_if(v, [](int i) { return i == 3; }));
    CHECK_FALSE(bricks::contains_if(v, [](int i) { return i == 4; }));
    CHECK_THROWS_AS(bricks::contains_if(v,
                                        [](int) {
                                          throw std::logic_error("Test error");
                                          return false;
                                        }),
                    std::logic_error);
  }

  SUBCASE("example")
  {
    /// [contains_if-example]
    std::vector<int> vec = {1, 2, 3, 4, 5};
    INFO(bricks::contains_if(vec, [](int i) { return i % 2 == 0; }));  // prints true
    /// [contains_if-example]
  }
}

TEST_SUITE_END();
