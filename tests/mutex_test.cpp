#include <doctest/doctest.h>

#include <algorithm>
#include <bricks/mutex.hpp>
#include <vector>

TEST_SUITE_BEGIN("[mutex]");

TEST_CASE("example")
{
  /// [mutex-example]
  bricks::mutex<std::vector<int>> vec{std::in_place, {3, 2, 1}};

  vec.lock()->push_back(4);  // Guaranteed to be thread safe write.

  INFO("Vector size: ", vec.lock()->size());  // Guaranteed to be thread safe read.

  {
    auto lock_guard = vec.lock();
    std::sort(lock_guard->begin(), lock_guard->end());
  }  // Lock is released here.
  /// [mutex-example]
}

TEST_CASE("underlying constructor works")
{
  bricks::mutex<std::vector<int>> c(std::in_place, 3, 1);
  auto r = c.lock();
  CHECK(r->size() == 3);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 1);
  CHECK(r->at(2) == 1);
}

TEST_CASE("underlying constructor works with initializer list")
{
  bricks::mutex<std::vector<int>> c({1, 2, 3});
  auto r = c.lock();
  CHECK(r->size() == 3);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);
}

TEST_CASE("locking is read-write")
{
  bricks::mutex<std::vector<int>> c({1, 2, 3});
  auto r = c.lock();
  CHECK(r->size() == 3);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);
  r->push_back(4);
  CHECK(r->size() == 4);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);
  CHECK(r->at(3) == 4);
}

TEST_CASE("locking unlocks at end of scope")
{
  bricks::mutex<std::vector<int>> c({1, 2, 3});
  {
    auto r = c.lock();
    CHECK(r->size() == 3);
    CHECK(r->at(0) == 1);
    CHECK(r->at(1) == 2);
    CHECK(r->at(2) == 3);
    r->push_back(4);
    CHECK(r->size() == 4);
    CHECK(r->at(0) == 1);
    CHECK(r->at(1) == 2);
    CHECK(r->at(2) == 3);
    CHECK(r->at(3) == 4);
  }

  auto r = c.lock();
  CHECK(r->size() == 4);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);
  CHECK(r->at(3) == 4);

  std::vector<int> v({1, 2, 3, 4});
  CHECK(v == *r);
}

TEST_CASE("locking from const context only allows reading")
{
  const bricks::mutex<std::vector<int>> c({1, 2, 3});
  auto r = c.lock();
  CHECK(r->size() == 3);
  CHECK(r->at(0) == 1);
  CHECK(r->at(1) == 2);
  CHECK(r->at(2) == 3);

  // Copying the underlying vector is allowed.
  auto v1{*r};
  CHECK(v1 == *r);
  // r->push_back(4); // This should not compile.
}

TEST_SUITE_END();
