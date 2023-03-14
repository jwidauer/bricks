#pragma once

#include "detail/filter.hpp"
namespace bricks {

/**
 * @brief Filter a range.
 *
 * This function creates an iterator adapter that filters the values of a range. The range can be
 * any type that has a begin and end function. Each element in the iterator adapter is a value that
 * satisfies the predicate.
 *
 * Example:
 * @snippet filter_test.cpp filter-example
 *
 * @param range The range to filter.
 * @param predicate The predicate to filter with.
 * @return auto The iterator adapter.
 */
template <typename Range, typename UnaryPredicate>
auto filter(Range& range, UnaryPredicate predicate) -> detail::filterer<Range, UnaryPredicate>
{
  return detail::filterer<Range, UnaryPredicate>{range, predicate};
}

}  // namespace bricks
