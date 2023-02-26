#pragma once

#include <type_traits>

namespace bricks::detail {

template <typename T, typename mutex_type>
class write_guard {
 public:
  write_guard(T& this_in, mutex_type& mutex_in) noexcept : this_(this_in), mutex_(mutex_in)
  {
    mutex_.lock();
  }

  ~write_guard() noexcept { mutex_.unlock(); }

  write_guard(const write_guard&) = delete;
  auto operator=(const write_guard&) -> write_guard& = delete;

  write_guard(write_guard&&) noexcept = default;
  auto operator=(write_guard&&) noexcept -> write_guard& = default;

  template <typename U = T, typename std::enable_if_t<!std::is_const_v<U>, bool> = true>
  auto operator->() noexcept -> typename T::value_type*
  {
    return &this_;
  }
  auto operator->() const noexcept -> const typename T::value_type* { return &this_; }

  template <typename U = T, typename std::enable_if_t<!std::is_const_v<U>, bool> = true>
  auto operator*() noexcept -> typename T::value_type&
  {
    return this_;
  }
  auto operator*() const noexcept -> const typename T::value_type& { return this_; }

 private:
  T& this_;
  mutex_type& mutex_;
};

}  // namespace bricks::detail
