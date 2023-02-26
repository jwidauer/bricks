#pragma once

#include <shared_mutex>

#include "detail/read_guard.hpp"
#include "detail/write_guard.hpp"

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
template <class Class>
class rw_lock : private Class {
 private:
  friend class detail::read_guard<rw_lock>;
  friend class detail::write_guard<rw_lock, std::shared_mutex>;

 public:
  using value_type = Class;

  using read_guard = detail::read_guard<rw_lock>;
  using write_guard = detail::write_guard<rw_lock, std::shared_mutex>;

  using Class::Class;

  /**
   * @brief Locks the resource for reading, blocking until the lock is acquired.
   *
   * @return read_guard An RAII style read guard, which will release the lock when
   * it goes out of scope.
   */
  auto read() const noexcept -> read_guard { return read_guard{*this, mutex_}; }

  /**
   * @brief Locks the resource for writing, blocking until the lock is acquired.
   *
   * @return write_guard An RAII style write guard, which will release the lock
   * when it goes out of scope.
   */
  auto write() noexcept -> write_guard { return write_guard{*this, mutex_}; }

 private:
  mutable std::shared_mutex mutex_;
};

}  // namespace bricks
