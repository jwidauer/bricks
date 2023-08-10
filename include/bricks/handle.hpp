#pragma once

#include <memory>

namespace bricks {

/** @brief A deleter is a function that deletes a pointer.
 *
 * @tparam T The type of the pointer.
 * @tparam fn The deleter function.
 */
template <typename T, auto fn>
struct deleter {
  void operator()(T* p) { fn(p); }
};

/** @brief A handle is a unique_ptr with a custom deleter.
 *
 * Example:
 * @snippet tests/handle_test.cpp handle-example
 *
 * @tparam T The type of the handle.
 * @tparam fn The deleter function.
 */
template <typename T, auto fn>
using handle = std::unique_ptr<T, deleter<T, fn>>;

}  // namespace bricks
