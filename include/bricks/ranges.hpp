#pragma once

#include "detail/enumerate.hpp"
#include "detail/filter.hpp"
#include "detail/reverse.hpp"
#include "detail/zip.hpp"

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

/**
 * @brief Filter a range.
 *
 * This function creates an iterator adapter that filters the values of a range. The range can be
 * any type that has a begin and end function. Each element in the iterator adapter is a value
 * that satisfies the predicate.
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

/**
 * @brief A zip iterator adapter
 *
 * This iterator allows to iterate over multiple containers at the same time.
 * It is similar to the zip function in python.
 * The iterator will iterate over the shortest container.
 * The iterator will dereference to a tuple of references to the elements of the containers.
 *
 * Example:
 * @snippet zip_test.cpp zip-example
 */
template <typename... Ranges>
auto zip(Ranges&&... t)
{
  return detail::zipper<Ranges...>{std::forward<Ranges>(t)...};
}

}  // namespace bricks
