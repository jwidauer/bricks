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
 * @code
 * std::vector<int> a{1, 2, 3};
 * std::vector<int> b{4, 5, 6};
 * for (auto [x, y] : bricks::zip(a, b)) {
 *  std::cout << x << ", " << y << std::endl;  // prints 1, 4, 2, 5, 3, 6
 * }
 * @endcode
 */
template <typename... Ranges>
auto zip(Ranges&&... t)
{
  return detail::zipper<Ranges...>{std::forward<Ranges>(t)...};
}

}  // namespace bricks
