#pragma once

#include <initializer_list>
#include <mutex>
#include <type_traits>
#include <utility>

#include "detail/lock_guard.hpp"

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
template <typename T>
struct mutex {
  using value_type = T;

  using lock_guard = detail::lock_guard<value_type, std::mutex, std::unique_lock>;
  using const_lock_guard = detail::lock_guard<const value_type, std::mutex, std::unique_lock>;

  mutex() noexcept(std::is_nothrow_default_constructible_v<T>) = default;

  mutex(const mutex&) = delete;
  auto operator=(const mutex&) -> mutex& = delete;

  mutex(mutex&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
      : mutex(other, other.lock())
  {
  }
  auto operator=(mutex&& other) noexcept(std::is_nothrow_move_assignable_v<T>) -> mutex&
  {
    if (this == &other) return *this;

    std::scoped_lock mutex_lock{mutex_, other.mutex_};
    data_ = std::move(other.data_);

    return *this;
  }

  explicit mutex(const T& data) noexcept(std::is_nothrow_copy_constructible_v<T>) : data_{data} {}
  explicit mutex(T&& data) noexcept(std::is_nothrow_move_constructible_v<T>)
      : data_{std::move(data)}
  {
  }

  template <typename... Args>
  explicit mutex(std::in_place_t /* inplace */, Args&&... args) : data_(std::forward<Args>(args)...)
  {
  }

  template <typename U, typename... Args>
  explicit mutex(
      std::in_place_t /* inplace */, std::initializer_list<U> il,
      Args&&... args) noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>,
                                                               Args...>)
      : data_(il, std::forward<Args>(args)...)
  {
  }

  /**
   * @brief Locks the mutex, blocking until the lock is acquired.
   *
   * @return lock_guard An RAII style lock guard, which will release the lock when it goes out of
   * scope.
   */
  auto lock() noexcept -> lock_guard { return lock_guard{data_, mutex_}; }
  auto lock() const noexcept -> const_lock_guard { return const_lock_guard{data_, mutex_}; }

 private:
  friend lock_guard;
  friend const_lock_guard;

  mutex(mutex&& other, lock_guard /*lock*/) noexcept(std::is_nothrow_move_constructible_v<T>)
      : data_{std::move(other.data_)}
  {
  }

  mutable std::mutex mutex_;
  T data_;
};

}  // namespace bricks
