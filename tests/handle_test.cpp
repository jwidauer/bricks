#include <doctest/doctest.h>

#include <bricks/handle.hpp>
#include <cstdio>

TEST_SUITE_BEGIN("[handle]");

TEST_CASE("handle")
{
  SUBCASE("example")
  {
    /// [handle-example]
    using file = bricks::handle<FILE, fclose>;
    auto f = file{fopen("test.txt", "w")};
    // f is automatically closed when it goes out of scope
    /// [handle-example]
  }
}

TEST_SUITE_END();
