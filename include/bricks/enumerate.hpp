#pragma once

#include <cstddef>
#include <iterator>

namespace bricks {

/**
 * @brief Enumerate a range.
 *
 * This function creates an enumerator from a range. The range can be any type
 * that has a begin and end function. The enumerator can be used in a range-based
 * for loop.
 * Each element in the enumerator is a pair of the index and the value.
 *
 * Example:
 * ```cpp
 * std::vector<int> v{1, 2, 3, 4, 5};
 * for (auto [i, e] : bricks::enumerate(v)) {
 *   std::cout << i << ": " << e << std::endl; // prints 0: 1, 1: 2, 2: 3, 3: 4, 4: 5
 * }
 * ```
 *
 * @tparam Range The type of range to enumerate.
 * @param range The range to enumerate.
 * @return auto An enumerator.
 */
template <typename Range>
auto enumerate(Range& range)
{
  using iter_t = decltype(std::begin(range));

  class pseudo_iter {
   private:
    size_t counter_{0};
    iter_t& iter_;

   public:
    explicit pseudo_iter(iter_t& iter) : iter_(iter) {}

    auto operator++() -> pseudo_iter&
    {
      ++iter_;
      ++counter_;
      return *this;
    }

    auto operator!=(const iter_t& other) const -> bool { return iter_ != other; }

    auto operator*() const -> std::pair<size_t, decltype(*iter_)> { return {counter_, *iter_}; }
  };

  class enumerator {
   private:
    iter_t begin_;
    const iter_t end_;

   public:
    explicit enumerator(Range& range) : begin_{std::begin(range)}, end_{std::end(range)} {}
    auto begin() -> pseudo_iter { return pseudo_iter{begin_}; }
    auto end() const -> const iter_t& { return end_; }
  };

  return enumerator{range};
}

}  // namespace bricks
