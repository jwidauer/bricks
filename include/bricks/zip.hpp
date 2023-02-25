#pragma once

#include "detail/zip.hpp"

namespace bricks {

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
