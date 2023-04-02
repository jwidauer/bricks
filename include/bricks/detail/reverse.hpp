#pragma once

#include <iterator>

namespace bricks::detail {

template <typename Range>
class reverse_iter {
 private:
  typename Range::reverse_iterator iter_;

 public:
  using difference_type = std::ptrdiff_t;
  using value_type = typename Range::value_type;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;

  explicit reverse_iter(typename Range::reverse_iterator iter) : iter_{iter} {}

  auto operator++() -> reverse_iter&
  {
    ++iter_;
    return *this;
  }

  auto operator++(int) -> reverse_iter
  {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  auto operator==(const reverse_iter& other) const -> bool { return iter_ == other.iter_; }
  auto operator!=(const reverse_iter& other) const -> bool { return !(*this == other); }

  auto operator*() const -> value_type& { return *iter_; }
};

template <typename Range>
class reverser {
 private:
  Range& range_;

 public:
  explicit reverser(Range& range) : range_{range} {}

  auto begin() -> reverse_iter<Range> { return reverse_iter<Range>{std::rbegin(range_)}; }
  auto end() -> reverse_iter<Range> { return reverse_iter<Range>{std::rend(range_)}; }
};

}  // namespace bricks::detail
