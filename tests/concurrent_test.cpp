#include <doctest/doctest.h>

#include <bricks/concurrent.hpp>
#include <thread>
#include <vector>

#include "test_error.hpp"

TEST_SUITE_BEGIN("[concurrent]");

TEST_CASE("Underlying constructor works")
{
  bricks::concurrent<std::vector<int>> c(3, 1);
  auto r = c.read();
  CHECK(r->size() == 3);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 1);
  CHECK(r->at(2) == 1);
}

TEST_CASE("Underlying constructor works with initializer list")
{
  bricks::concurrent<std::vector<int>> c({1, 2, 3});
  auto r = c.read();
  CHECK(r->size() == 3);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);
}

TEST_CASE("Concurrent read from same thread doesn't block")
{
  bricks::concurrent<std::vector<int>> c({1, 2, 3});
  auto r = c.read();
  CHECK(r->size() == 3);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);

  auto r2 = c.read();
  CHECK(r2->size() == 3);
  CHECK(r2->at(0) == 1);
  CHECK(r2->at(1) == 2);
  CHECK(r2->at(2) == 3);
}

TEST_CASE("Reading is read only")
{
  bricks::concurrent<std::vector<int>> c({1, 2, 3});
  auto r = c.read();
  CHECK(r->size() == 3);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);

  // r->push_back(4); // Doesn't compile
}

TEST_CASE("Writing is read-write")
{
  bricks::concurrent<std::vector<int>> c({1, 2, 3});
  auto w = c.write();

  w->push_back(4);
  CHECK(w->size() == 4);
  CHECK(w->at(0) == 1);
  CHECK(w->at(1) == 2);
  CHECK(w->at(2) == 3);
  CHECK(w->at(3) == 4);
}

TEST_CASE("Reading unlocks at end of scope")
{
  bricks::concurrent<std::vector<int>> c({1, 2, 3});
  {
    auto r = c.read();
    CHECK(r->size() == 3);
    CHECK(r->at(0) == 1);
    CHECK(r->at(1) == 2);
    CHECK(r->at(2) == 3);
  }

  auto w = c.write();
  w->push_back(4);
  CHECK(w->size() == 4);
  CHECK(w->at(0) == 1);
  CHECK(w->at(1) == 2);
  CHECK(w->at(2) == 3);
  CHECK(w->at(3) == 4);
}

TEST_CASE("Writing unlocks at end of scope")
{
  bricks::concurrent<std::vector<int>> c({1, 2, 3});
  {
    auto w = c.write();
    w->push_back(4);
    CHECK(w->size() == 4);
    CHECK(w->at(0) == 1);
    CHECK(w->at(1) == 2);
    CHECK(w->at(2) == 3);
    CHECK(w->at(3) == 4);
  }

  auto r = c.read();
  CHECK(r->size() == 4);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);
  CHECK(r->at(3) == 4);
}

TEST_CASE("Writing unlocks at end of scope even if exception is thrown" * doctest::timeout(0.5))
{
  bricks::concurrent<std::vector<int>> c({1, 2, 3});
  try {
    auto w = c.write();
    w->push_back(4);
    CHECK(w->size() == 4);
    CHECK(w->at(0) == 1);
    CHECK(w->at(1) == 2);
    CHECK(w->at(2) == 3);
    CHECK(w->at(3) == 4);
    throw bricks::test::test_error("Test exception");
  } catch (const bricks::test::test_error& e) {
    CHECK(e.what() == std::string("Test exception"));
  }

  auto r = c.read();
  CHECK(r->size() == 4);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);
  CHECK(r->at(3) == 4);
}

TEST_CASE("Reading unlocks at end of scope even if exception is thrown" * doctest::timeout(0.5))
{
  bricks::concurrent<std::vector<int>> c({1, 2, 3});
  try {
    auto r = c.read();
    CHECK(r->size() == 3);
    CHECK(r->at(0) == 1);
    CHECK(r->at(1) == 2);
    CHECK(r->at(2) == 3);
    throw bricks::test::test_error("Test exception");
  } catch (const bricks::test::test_error& e) {
    CHECK(e.what() == std::string("Test exception"));
  }

  auto w = c.write();
  w->push_back(4);
  CHECK(w->size() == 4);
  CHECK(w->at(0) == 1);
  CHECK(w->at(1) == 2);
  CHECK(w->at(2) == 3);
  CHECK(w->at(3) == 4);
}

TEST_CASE("Writing from separate threads is race free")
{
  bricks::concurrent<std::vector<int>> c({1, 2, 3});
  std::thread t1([&c]() {
    auto w = c.write();
    w->push_back(4);
  });
  std::thread t2([&c]() {
    auto w = c.write();
    w->push_back(5);
  });
  t1.join();
  t2.join();

  auto r = c.read();
  CHECK(r->size() == 5);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);
}

TEST_CASE("Writing blocks reading from separate threads")
{
  bricks::concurrent<std::vector<int>> c({1, 2, 3});
  std::thread t1([&c]() {
    auto w = c.write();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    w->push_back(4);
  });
  std::thread t2([&c]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    auto r = c.read();
    CHECK(r->size() == 4);
    CHECK(r->at(0) == 1);
    CHECK(r->at(1) == 2);
    CHECK(r->at(2) == 3);
    CHECK(r->at(3) == 4);
  });
  t1.join();
  t2.join();
}

TEST_SUITE_END();
