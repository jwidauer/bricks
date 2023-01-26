#pragma once

#include <iterator>

namespace bricks {

/**
 * @brief Create a reverse iterator from a range.
 *
 * This function creates a reverse iterator from a range. The range can be any
 * type that has a rbegin and rend function. The reverse iterator can be used in
 * a range-based for loop.
 *
 * Example:
 * ```
 * std::vector<int> v{1, 2, 3, 4, 5};
 * for (auto e : bricks::reverse(v)) {
 *  std::cout << e << std::endl; // prints 5, 4, 3, 2, 1
 * }
 * ```
 *
 * @tparam Range The type of range to reverse.
 * @param range The range to reverse.
 * @return auto A reverse iterator.
 */
template <typename Range>
auto reverse(Range& range)
{
  using iter_t = decltype(std::rbegin(range));

  class reverser {
   private:
    iter_t begin_;
    const iter_t end_;

   public:
    explicit reverser(Range& range) : begin_{std::rbegin(range)}, end_{std::rend(range)} {}
    auto begin() -> iter_t& { return begin_; }
    auto end() const -> const iter_t& { return end_; }
  };

  return reverser{range};
}

}  // namespace bricks
