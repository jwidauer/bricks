#pragma once

namespace bricks::detail {

template <class T, class MutexType, template <class> class LockType>
class lock_guard {
 public:
  lock_guard(T& data, MutexType& mutex) noexcept : lock_{mutex}, data_{data} {}
  ~lock_guard() noexcept = default;

  lock_guard(const lock_guard&) = delete;
  auto operator=(const lock_guard&) -> lock_guard& = delete;

  lock_guard(lock_guard&&) noexcept = default;
  auto operator=(lock_guard&&) noexcept -> lock_guard& = default;

  auto operator->() noexcept -> T* { return &data_; }
  auto operator->() const noexcept -> const T* { return &data_; }

  auto operator*() noexcept -> T& { return data_; }
  auto operator*() const noexcept -> const T& { return data_; }

 private:
  LockType<MutexType> lock_;
  T& data_;
};

}  // namespace bricks::detail
