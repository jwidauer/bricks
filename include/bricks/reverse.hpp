#pragma once

#include "detail/reverse.hpp"

namespace bricks {

/**
 * @brief Create a reverse iterator from a range.
 *
 * This function creates a reverse iterator from a range. The range can be any
 * type that has a rbegin and rend function.
 *
 * Example:
 * @snippet reverse_test.cpp reverse-example
 *
 * @tparam Range The type of range to reverse.
 * @param range The range to reverse.
 * @return auto A reverse iterator.
 */
template <typename Range>
auto reverse(Range& range)
{
  return detail::reverser<Range>{range};
}

}  // namespace bricks
