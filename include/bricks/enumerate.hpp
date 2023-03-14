#pragma once

#include "detail/enumerate.hpp"

namespace bricks {

/**
 * @brief Enumerate a range.
 *
 * This function creates an iterator adapter that enumerates the values of a range. The range can be
 * any type that has a begin and end function. Each element in the enumerator is a pair of the index
 * and the value.
 *
 * Example:
 * @snippet enumerate_test.cpp enumerate-example
 *
 * @tparam Range The type of range to enumerate.
 * @param range The range to enumerate.
 * @return auto An enumerator.
 */
template <typename Range>
auto enumerate(Range& range)
{
  return detail::enumerator<Range>{range};
}

}  // namespace bricks
