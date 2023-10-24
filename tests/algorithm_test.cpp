#include <doctest/doctest.h>

#include <bricks/algorithm.hpp>
#include <chrono>
#include <functional>
#include <future>
#include <map>
#include <string>
#include <system_error>
#include <thread>
#include <vector>

TEST_SUITE_BEGIN("[algorithm]");

TEST_CASE("keys")
{
  SUBCASE("test")
  {
    std::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
    auto keys = bricks::keys(m);
    CHECK(keys.size() == 3);
    CHECK(keys[0] == 1);
    CHECK(keys[1] == 2);
    CHECK(keys[2] == 3);
  }

  SUBCASE("example")
  {
    /// [keys-example]
    std::map<std::string, int, std::less<>> map = {{"a", 1}, {"b", 2}, {"c", 3}};
    auto keys = bricks::keys(map);
    for (const auto& key : keys) {
      INFO(key);  // prints "a", "b", "c"
    }
    /// [keys-example]
  }
}

TEST_CASE("values")
{
  SUBCASE("test")
  {
    std::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};
    auto values = bricks::values(m);
    CHECK(values.size() == 3);
    CHECK(values[0] == "one");
    CHECK(values[1] == "two");
    CHECK(values[2] == "three");
  }

  SUBCASE("example")
  {
    /// [values-example]
    std::map<std::string, int, std::less<>> map = {{"a", 1}, {"b", 2}, {"c", 3}};
    auto values = bricks::values(map);
    for (const auto& value : values) {
      INFO(value);  // prints 1, 2, 3
    }
    /// [values-example]
  }
}

TEST_CASE("bind_front")
{
  SUBCASE("test")
  {
    auto add = [](int a, int b) { return a + b; };
    auto add_1 = bricks::bind_front(add, 1);
    CHECK(add_1(2) == 3);
  }

  SUBCASE("example")
  {
    /// [bind_front-example]
    auto add = [](int a, int b) { return a + b; };
    auto add_1 = bricks::bind_front(add, 1);
    INFO(add_1(2));  // prints 3
    /// [bind_front-example]
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

  SUBCASE("example")
  {
    /// [is_ready_after-example]
    auto future = std::async(std::launch::async, []() {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      return 42;
    });
    // Will print "Future is ready!" after 10 milliseconds.
    if (bricks::is_ready_after(future, std::chrono::seconds(10))) {
      INFO("Future is ready!");
    }
    /// [is_ready_after-example]
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

  SUBCASE("example")
  {
    /// [is_ready_at-example]
    auto future = std::async(std::launch::async, []() {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      return 42;
    });
    // Will print "Future is ready!" after 10 milliseconds.
    if (bricks::is_ready_at(future, std::chrono::steady_clock::now() + std::chrono::seconds(10))) {
      INFO("Future is ready!");
    }
    /// [is_ready_at-example]
  }
}

TEST_SUITE_END();
