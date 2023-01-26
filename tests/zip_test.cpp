#include <doctest/doctest.h>

#include <bricks/zip.hpp>
#include <numeric>
#include <vector>

TEST_SUITE_BEGIN("[zip]");

TEST_CASE("same size vectors")
{
  std::vector<int> v1 = {1, 2, 3};
  std::vector<int> v2 = {4, 5, 6};
  auto zipped = bricks::zip(v1, v2);
  auto it = zipped.begin();

  REQUIRE(it != zipped.end());
  auto [first, second] = *it;
  CHECK(first == 1);
  CHECK(second == 4);
  ++it;

  std::tie(first, second) = *it;
  CHECK(first == 2);
  CHECK(second == 5);
  ++it;

  std::tie(first, second) = *it;
  CHECK(first == 3);
  CHECK(second == 6);
  ++it;

  CHECK(it == zipped.end());
}

TEST_CASE("different size vectors")
{
  std::vector<int> v1 = {1, 2, 3};
  std::vector<int> v2 = {4, 5};

  SUBCASE("first is smaller")
  {
    auto zipped = bricks::zip(v1, v2);
    auto it = zipped.begin();
    auto [first, second] = *it;
    CHECK(first == 1);
    CHECK(second == 4);
    ++it;

    std::tie(first, second) = *it;
    CHECK(first == 2);
    CHECK(second == 5);
    ++it;

    CHECK(it == zipped.end());
  }

  SUBCASE("second is smaller")
  {
    auto zipped = bricks::zip(v2, v1);
    auto it = zipped.begin();
    auto [first, second] = *it;
    CHECK(first == 4);
    CHECK(second == 1);
    ++it;

    std::tie(first, second) = *it;
    CHECK(first == 5);
    CHECK(second == 2);
    ++it;

    CHECK(it == zipped.end());
  }
}

TEST_CASE("empty vectors")
{
  std::vector<int> v1 = {};
  std::vector<int> v2 = {};
  auto zipped = bricks::zip(v1, v2);
  auto it = zipped.begin();
  CHECK(it == zipped.end());
}

TEST_CASE("operator++()")
{
  std::vector<int> v1 = {1, 2, 3};
  std::vector<int> v2 = {4, 5, 6};
  auto zipped = bricks::zip(v1, v2);
  auto it = zipped.begin();

  REQUIRE(it != zipped.end());
  auto [first, second] = *it;
  CHECK(first == 1);
  CHECK(second == 4);
  ++it;

  std::tie(first, second) = *it;
  CHECK(first == 2);
  CHECK(second == 5);
  ++it;

  std::tie(first, second) = *it;
  CHECK(first == 3);
  CHECK(second == 6);
  ++it;

  CHECK(it == zipped.end());
}

TEST_CASE("operator++(int)")
{
  std::vector<int> v1 = {1, 2, 3};
  std::vector<int> v2 = {4, 5, 6};
  auto zipped = bricks::zip(v1, v2);
  auto it = zipped.begin();

  REQUIRE(it != zipped.end());
  auto [first, second] = *it;
  CHECK(first == 1);
  CHECK(second == 4);
  it++;

  std::tie(first, second) = *it;
  CHECK(first == 2);
  CHECK(second == 5);
  it++;

  std::tie(first, second) = *it;
  CHECK(first == 3);
  CHECK(second == 6);
  it++;

  CHECK(it == zipped.end());
}

TEST_CASE("works with range based for loop")
{
  std::vector<int> v1 = {1, 2, 3};
  std::vector<int> v2 = {4, 5, 6};
  auto zipped = bricks::zip(v1, v2);
  for (auto [first, second] : zipped) {
    CHECK(first == second - 3);
  }
}

TEST_CASE("works with algorithms")
{
  std::vector<int> v1 = {1, 2, 3};
  std::vector<int> v2 = {4, 5, 6};
  auto zipped = bricks::zip(v1, v2);
  auto sum = std::accumulate(zipped.begin(), zipped.end(), 0, [](int acc, const auto& pair) {
    auto [first, second] = pair;
    return acc + first + second;
  });
  CHECK(sum == 21);
}

TEST_SUITE_END();
