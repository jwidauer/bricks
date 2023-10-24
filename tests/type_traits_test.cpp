#include <bricks/type_traits.hpp>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/// [has_find-example]
struct foo {
  [[nodiscard]] auto find(int /* unused */) const -> int { return a_; };

 private:
  int a_ = 0;
};

struct bar {
  [[nodiscard]] auto find(int /* unused */) -> int& { return a_; };

 private:
  int a_ = 0;
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

static_assert(bricks::is_map_like_v<std::map<std::string, int>>);
static_assert(bricks::is_map_like_v<std::unordered_map<int, std::string>>);
static_assert(std::negation_v<bricks::is_map_like<std::vector<int>>>);
static_assert(std::negation_v<bricks::is_map_like<std::set<int>>>);
static_assert(std::negation_v<bricks::is_map_like<std::unordered_set<int>>>);

static_assert(bricks::is_set_like_v<std::set<std::string>>);
static_assert(bricks::is_set_like_v<std::unordered_set<std::string>>);
static_assert(std::negation_v<bricks::is_set_like<std::map<std::string, int>>>);
static_assert(std::negation_v<bricks::is_set_like<std::unordered_map<std::string, int>>>);
static_assert(std::negation_v<bricks::is_set_like<std::vector<std::string>>>);
