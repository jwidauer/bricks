#pragma once

#include <initializer_list>
#include <mutex>
#include <shared_mutex>

#include "detail/lock_guard.hpp"

namespace bricks {

/**
 * @brief A reader-writer lock.
 *
 * This class implements a reader-writer lock around a shared resource. The
 * resource can be read by multiple threads at the same time, but only one thread
 * can write to the resource at a time. Read access is read only, write access is
 * read-write.
 *
 * Example usage:
 * @snippet rw_lock_test.cpp rw_lock-example
 *
 * Very similar to rust's
 * [std::sync::RwLock](https://doc.rust-lang.org/std/sync/struct.RwLock.html).
 *
 *
 * @tparam Class The data type to be protected.
 */
template <class T>
struct rw_lock {
  using value_type = T;

  using read_guard = detail::lock_guard<const value_type, std::shared_mutex, std::shared_lock>;
  using write_guard = detail::lock_guard<value_type, std::shared_mutex, std::unique_lock>;

  rw_lock() noexcept(std::is_nothrow_default_constructible_v<T>) = default;

  rw_lock(const rw_lock&) = delete;
  auto operator=(const rw_lock&) -> rw_lock& = delete;

  rw_lock(rw_lock&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
      : rw_lock{other, other.write()}
  {
  }
  auto operator=(rw_lock&& other) noexcept(std::is_nothrow_move_assignable_v<T>) -> rw_lock&
  {
    if (this == &other) return *this;

    std::scoped_lock lock{mutex_, other.mutex_};
    data_ = std::move(other.data_);

    return *this;
  }

  explicit rw_lock(const T& data) noexcept(std::is_nothrow_copy_constructible_v<T>) : data_{data} {}
  explicit rw_lock(T&& data) noexcept(std::is_nothrow_move_constructible_v<T>)
      : data_{std::move(data)}
  {
  }

  template <class... Args>
  explicit rw_lock(std::in_place_t /* inplace */,
                   Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
      : data_(std::forward<Args>(args)...)
  {
  }

  template <class U, class... Args>
  explicit rw_lock(
      std::in_place_t /* inplace */, std::initializer_list<U> il,
      Args&&... args) noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>,
                                                               Args...>)
      : data_(il, std::forward<Args>(args)...)
  {
  }

  /**
   * @brief Locks the resource for reading, blocking until the lock is acquired.
   *
   * @return read_guard An RAII style read guard, which will release the lock when
   * it goes out of scope.
   */
  auto read() const noexcept -> read_guard { return read_guard{data_, mutex_}; }

  /**
   * @brief Locks the resource for writing, blocking until the lock is acquired.
   *
   * @return write_guard An RAII style write guard, which will release the lock
   * when it goes out of scope.
   */
  auto write() noexcept -> write_guard { return write_guard{data_, mutex_}; }

 private:
  friend read_guard;
  friend write_guard;

  rw_lock(rw_lock&& other, write_guard&& /*lock*/) noexcept(std::is_nothrow_move_constructible_v<T>)
      : data_{std::move(other.data_)}
  {
  }

  mutable std::shared_mutex mutex_;
  T data_;
};

}  // namespace bricks
