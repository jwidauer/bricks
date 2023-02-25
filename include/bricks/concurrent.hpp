#pragma once

#include <shared_mutex>

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
 * @snippet concurrent_test.cpp concurrent-example
 *
 * Very similar to rust's
 * [std::sync::RwLock](https://doc.rust-lang.org/std/sync/struct.RwLock.html).
 *
 *
 * @tparam Class The data type to be protected.
 */
template <class Class>
class concurrent : private Class {
 private:
  // Forward declarations
  class read_guard;
  class write_guard;

 public:
  using Class::Class;

  /**
   * @brief Locks the resource for reading, blocking until the lock is acquired.
   *
   * @return read_guard An RAII style read guard, which will release the lock when
   * it goes out of scope.
   */
  auto read() const noexcept -> read_guard { return read_guard{this, &mutex_}; }

  /**
   * @brief Locks the resource for writing, blocking until the lock is acquired.
   *
   * @return write_guard An RAII style write guard, which will release the lock
   * when it goes out of scope.
   */
  auto write() noexcept -> write_guard { return write_guard{this, &mutex_}; }

 private:
  class read_guard {
   public:
    read_guard(const concurrent* const this_in, std::shared_mutex* const mutex_in) noexcept
        : this_(this_in), mutex_(mutex_in)
    {
      mutex_->lock_shared();
    }

    ~read_guard() noexcept { mutex_->unlock_shared(); }

    read_guard(const read_guard&) = default;
    auto operator=(const read_guard&) -> read_guard& = default;

    auto operator->() const noexcept -> const Class* { return this_; }

    auto operator*() const noexcept -> const Class& { return *this_; }

   private:
    const concurrent* const this_;
    std::shared_mutex* mutex_;
  };

  class write_guard {
   public:
    write_guard(concurrent* const this_in, std::shared_mutex* const mutex_in) noexcept
        : this_(this_in), mutex_(mutex_in)
    {
      mutex_->lock();
    }

    ~write_guard() noexcept { mutex_->unlock(); }

    write_guard(const write_guard&) = default;
    auto operator=(const write_guard&) -> write_guard& = default;

    auto operator->() noexcept -> Class* { return this_; }

    auto operator->() const noexcept -> const Class* { return this_; }

    auto operator*() noexcept -> Class& { return *this_; }

    auto operator*() const noexcept -> const Class& { return *this_; }

   private:
    concurrent* const this_;
    std::shared_mutex* const mutex_;
  };

  mutable std::shared_mutex mutex_;
};

}  // namespace bricks
