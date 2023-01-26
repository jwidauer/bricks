#include <doctest/doctest.h>

#include <bricks/timer.hpp>

#include "string_makers.hpp"

TEST_SUITE_BEGIN("[timer]");

void check_token_result(bricks::timer::completion_token& token, uint8_t wait_time_ms)
{
  CHECK(token.valid());
  REQUIRE(token.wait_for(std::chrono::milliseconds(wait_time_ms)) == std::future_status::ready);
  CHECK_NOTHROW(token.get());
}

TEST_CASE("Can be default constructed")
{
  bricks::timer t;
  auto completion_future = t.start();
  check_token_result(completion_future, 1);
}

TEST_CASE("Can be started with a duration")
{
  bricks::timer t;
  auto completion_future = t.start(std::chrono::milliseconds(1));
  check_token_result(completion_future, 5);
}

TEST_CASE("Can be aborted")
{
  bricks::timer t;
  auto completion_future = t.start(std::chrono::milliseconds(100));
  CHECK(completion_future.valid());
  t.abort();
  check_token_result(completion_future, 1);
}

TEST_CASE("Can start multiple timers")
{
  bricks::timer t;
  auto completion_future_1 = t.start(std::chrono::milliseconds(1));
  auto completion_future_2 = t.start(std::chrono::milliseconds(1));
  check_token_result(completion_future_1, 5);
  check_token_result(completion_future_2, 5);
}

TEST_CASE("Can abort multiple timers")
{
  bricks::timer t;
  auto completion_future_1 = t.start(std::chrono::milliseconds(100));
  auto completion_future_2 = t.start(std::chrono::milliseconds(100));
  CHECK(completion_future_1.valid());
  CHECK(completion_future_2.valid());
  t.abort();
  check_token_result(completion_future_1, 1);
  check_token_result(completion_future_2, 1);
}

TEST_CASE("Can start a timer after another finished")
{
  bricks::timer t;
  auto completion_future = t.start(std::chrono::milliseconds(1));
  CHECK(completion_future.valid());
  check_token_result(completion_future, 5);

  completion_future = t.start(std::chrono::milliseconds(1));
  check_token_result(completion_future, 5);
}

TEST_CASE("Can be aborted and restarted")
{
  bricks::timer t;
  auto completion_future = t.start(std::chrono::milliseconds(100));
  CHECK(completion_future.valid());
  t.abort();
  check_token_result(completion_future, 1);

  completion_future = t.start(std::chrono::milliseconds(1));
  check_token_result(completion_future, 5);
}

TEST_CASE("Can be destroyed while a timer is running")
{
  bricks::timer t;
  auto completion_future = t.start(std::chrono::milliseconds(100));
  CHECK(completion_future.valid());
}

TEST_CASE("Calling abort twice does not cause a crash")
{
  bricks::timer t;
  CHECK_NOTHROW(t.abort());
  CHECK_NOTHROW(t.abort());
}

TEST_CASE("Negative durations are treated as 0")
{
  bricks::timer t;
  auto completion_future = t.start(std::chrono::milliseconds(-1));
  check_token_result(completion_future, 1);
}

TEST_SUITE_END();
