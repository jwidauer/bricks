#include <doctest/doctest.h>

#include <bricks/type_traits.hpp>
#include <vector>

/// [has_find-example]
struct foo {
  [[nodiscard]] auto find(int /* unused */) const -> int { return 0; };
};

struct bar {
  [[nodiscard]] auto find(int /* unused */) -> int { return 0; };
};

struct baz {};

static_assert(bricks::has_find_v<foo, int>);
static_assert(bricks::has_find_v<bar, int>);
static_assert(!bricks::has_find_v<baz, int>);
/// [has_find-example]

/// [is_iterator-example]
static_assert(!bricks::is_iterator_v<int>);
static_assert(bricks::is_iterator_v<std::vector<int>::iterator>);
static_assert(bricks::is_iterator_v<std::vector<int>::const_iterator>);
/// [is_iterator-example]
