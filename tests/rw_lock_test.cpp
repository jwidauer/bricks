#include <doctest/doctest.h>

#include <algorithm>
#include <bricks/rw_lock.hpp>
#include <thread>
#include <vector>

#include "test_error.hpp"

TEST_SUITE_BEGIN("[rw_lock]");

TEST_CASE("example")
{
  /// [rw_lock-example]
  bricks::rw_lock<std::vector<int>> vec{std::in_place, {3, 2, 1}};

  vec.write()->push_back(4);  // Guaranteed to be thread safe write.

  INFO("Vector size: ", vec.read()->size());  // Guaranteed to be thread safe read.

  // Many reader locks can be held at the same time.
  {
    auto r1 = vec.read();
    auto r2 = vec.read();

    const auto safe_read = vec.read();
    for (const auto& i : *safe_read) {
      INFO("Vector element: ", i);
    }
    INFO("Vector size: ", r1->size());
    INFO("First element: ", r2->front());
  }  // Read locks are released here.

  // Only one writer lock can be held at a time.
  {
    auto safe_write = vec.write();
    std::sort(safe_write->begin(), safe_write->end());
  }  // Write lock is released here.
  /// [rw_lock-example]
}

TEST_CASE("Underlying constructor works")
{
  bricks::rw_lock<std::vector<int>> c(std::in_place, 3, 1);
  auto r = c.read();
  CHECK(r->size() == 3);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 1);
  CHECK(r->at(2) == 1);
}

TEST_CASE("Underlying constructor works with initializer list")
{
  bricks::rw_lock<std::vector<int>> c({1, 2, 3});
  auto r = c.read();
  CHECK(r->size() == 3);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);
}

TEST_CASE("Concurrent read from same thread doesn't block")
{
  bricks::rw_lock<std::vector<int>> c({1, 2, 3});
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
  bricks::rw_lock<std::vector<int>> c({1, 2, 3});
  auto r = c.read();
  CHECK(r->size() == 3);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);

  // r->push_back(4); // Doesn't compile
}

TEST_CASE("Writing is read-write")
{
  bricks::rw_lock<std::vector<int>> c({1, 2, 3});
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
  bricks::rw_lock<std::vector<int>> c({1, 2, 3});
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
  bricks::rw_lock<std::vector<int>> c({1, 2, 3});
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
  bricks::rw_lock<std::vector<int>> c({1, 2, 3});
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
  bricks::rw_lock<std::vector<int>> c({1, 2, 3});
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
  bricks::rw_lock<std::vector<int>> c({1, 2, 3});
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
  bricks::rw_lock<std::vector<int>> c({1, 2, 3});
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

TEST_CASE("read lock can be moved")
{
  bricks::rw_lock<std::vector<int>> c({1, 2, 3});
  auto r1 = c.read();
  auto r2 = std::move(r1);
  CHECK(r2->size() == 3);
}

TEST_CASE("write lock can be moved")
{
  bricks::rw_lock<std::vector<int>> c({1, 2, 3});
  auto w1 = c.write();
  auto w2 = std::move(w1);
  w2->push_back(4);
  CHECK(w2->size() == 4);
}

TEST_SUITE_END();
