#pragma once

#include <cstddef>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <vector>

namespace bricks::detail {

/*  The index sequence is only used to deduce the Index sequence in the template
    declaration. It uses a fold expression which is applied to the indexes,
    using each expanded value to compare tuple value at that index. If any of
    the tuple elements are equal, the function will return true. */
template <typename... Args, std::size_t... Index>
auto any_match_impl(std::tuple<Args...> const& lhs, std::tuple<Args...> const& rhs,
                    std::index_sequence<Index...> /* unused */) -> bool
{
  return (... || (std::get<Index>(lhs) == std::get<Index>(rhs)));
}

/**
 * @brief Function for checking if any of the elements in a tuple match.
 */
template <typename... Args>
auto any_match(std::tuple<Args...> const& lhs, std::tuple<Args...> const& rhs) -> bool
{
  return any_match_impl(lhs, rhs, std::index_sequence_for<Args...>{});
}

/**
 * @brief Type used within the tuple of the zip_iterator.
 *
 * This is only here for the special case of the `std::vector<bool>`, which doesn't iterate
 * over references, but over values.
 */
template <typename Iter>
using tuple_value_type =
    std::conditional_t<std::is_same_v<Iter, std::vector<bool>::iterator> ||
                           std::is_same_v<Iter, std::vector<bool>::const_iterator>,
                       typename Iter::value_type, typename Iter::reference>;

template <typename... Iters>
class zip_iterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = std::tuple<tuple_value_type<Iters>...>;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

  zip_iterator() = delete;

  explicit zip_iterator(Iters&&... iters) : ierators_{std::move(iters)...} {}

  auto operator++() -> zip_iterator&
  {
    std::apply([](auto&&... args) { ((args++), ...); }, ierators_);
    return *this;
  }

  auto operator++(int) -> zip_iterator
  {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  auto operator==(zip_iterator const& other) { return any_match(ierators_, other.ierators_); }
  auto operator!=(zip_iterator const& other) { return !(*this == other); }

  auto operator*() const -> value_type
  {
    return std::apply([](auto&&... args) { return value_type(*args...); }, ierators_);
  }

 private:
  std::tuple<Iters...> ierators_;
};

/* std::decay needed because T is a reference, and is not a complete type */
template <typename T>
using select_iterator_for = std::conditional_t<std::is_const_v<std::remove_reference_t<T>>,
                                               typename std::decay_t<T>::const_iterator,
                                               typename std::decay_t<T>::iterator>;

template <typename... T>
class zipper {
 public:
  using iter_t = zip_iterator<select_iterator_for<T>...>;

  explicit zipper(T&&... args) : args_{args...} {}

  auto begin() const -> iter_t
  {
    return std::apply([](auto&&... args) { return iter_t(std::begin(args)...); }, args_);
  }
  auto end() const -> iter_t
  {
    return std::apply([](auto&&... args) { return iter_t(std::end(args)...); }, args_);
  }

 private:
  std::tuple<T...> args_;
};

}  // namespace bricks::detail
