#pragma once

#include <shared_mutex>

namespace bricks::detail {

template <typename T>
class read_guard {
 public:
  read_guard(const T& this_in, std::shared_mutex& mutex_in) noexcept
      : this_(this_in), mutex_(mutex_in)
  {
    mutex_.lock_shared();
  }

  ~read_guard() noexcept { mutex_.unlock_shared(); }

  read_guard(const read_guard&) = delete;
  auto operator=(const read_guard&) -> read_guard& = delete;

  read_guard(read_guard&&) noexcept = default;
  auto operator=(read_guard&&) noexcept -> read_guard& = default;

  auto operator->() const noexcept -> const typename T::value_type* { return &this_; }

  auto operator*() const noexcept -> const typename T::value_type& { return this_; }

 private:
  const T& this_;
  std::shared_mutex& mutex_;
};

}  // namespace bricks::detail
