#include <doctest/doctest.h>

#include <bricks/algorithm.hpp>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

TEST_SUITE_BEGIN("[algorithm]");

TEST_CASE("keys")
{
  std::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
  auto keys = bricks::keys(m);
  CHECK(keys.size() == 3);
  CHECK(keys[0] == 1);
  CHECK(keys[1] == 2);
  CHECK(keys[2] == 3);
}

TEST_CASE("values")
{
  std::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
  auto values = bricks::values(m);
  CHECK(values.size() == 3);
  CHECK(values[0] == "one");
  CHECK(values[1] == "two");
  CHECK(values[2] == "three");
}

TEST_CASE("bind_front")
{
  auto add = [](int a, int b) { return a + b; };
  auto add_1 = bricks::bind_front(add, 1);
  CHECK(add_1(2) == 3);
}

TEST_CASE("filter")
{
  SUBCASE("simple")
  {
    const std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto is_even = [](int i) { return i % 2 == 0; };
    const auto evens = bricks::filter(v, is_even);
    REQUIRE(evens.size() == 5);
    CHECK(evens[0] == 2);
    CHECK(evens[1] == 4);
    CHECK(evens[2] == 6);
    CHECK(evens[3] == 8);
    CHECK(evens[4] == 10);
  }

  SUBCASE("empty container")
  {
    const std::vector<int> v;
    auto is_even = [](int i) { return i % 2 == 1; };
    const auto evens = bricks::filter(v, is_even);
    REQUIRE(evens.size() == 0);
  }
}

TEST_CASE("is_ready_after")
{
  std::promise<int> p;
  SUBCASE("ready")
  {
    p.set_value(42);
    auto f = p.get_future();
    CHECK(bricks::is_ready_after(f, std::chrono::milliseconds(0)));
  }

  SUBCASE("not ready")
  {
    auto f = p.get_future();
    CHECK_FALSE(bricks::is_ready_after(f, std::chrono::milliseconds(0)));
  }
}

TEST_CASE("is_ready_at")
{
  std::promise<int> p;
  SUBCASE("ready")
  {
    p.set_value(42);
    auto f = p.get_future();
    CHECK(bricks::is_ready_at(f, std::chrono::steady_clock::now()));
  }

  SUBCASE("not ready")
  {
    auto f = p.get_future();
    CHECK_FALSE(bricks::is_ready_at(f, std::chrono::steady_clock::now()));
  }
}

TEST_SUITE_END();
