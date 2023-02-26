#pragma once

#include <mutex>
#include <type_traits>

#include "detail/write_guard.hpp"

namespace bricks {

/**
 * @brief A mutual exclusion primitive that can be used to protect shared data
 *
 * This mutex will block threads waiting for the lock to become available. The mutex can be created
 * by passing the data to be protected to the constructor. The data can be accessed by calling the
 * `lock()` method, which returns a RAII style lock guard. The lock guard can be used to access the
 * protected data.
 *
 * Example usage:
 * @snippet mutex_test.cpp mutex-example
 */
template <typename Class, typename std::enable_if_t<std::is_class_v<Class>, bool> = true>
class mutex : private Class {
 private:
  friend class detail::write_guard<mutex, std::mutex>;
  friend class detail::write_guard<const mutex, std::mutex>;

 public:
  using value_type = Class;

  using lock_guard = detail::write_guard<mutex, std::mutex>;
  using const_lock_guard = detail::write_guard<const mutex, std::mutex>;

  using Class::Class;

  /**
   * @brief Locks the mutex, blocking until the lock is acquired.
   *
   * @return lock_guard An RAII style lock guard, which will release the lock when it goes out of
   * scope.
   */
  auto lock() noexcept -> lock_guard { return lock_guard{*this, mutex_}; }
  auto lock() const noexcept -> const_lock_guard { return const_lock_guard{*this, mutex_}; }

 private:
  mutable std::mutex mutex_;
};

}  // namespace bricks
