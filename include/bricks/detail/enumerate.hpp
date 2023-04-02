#pragma once

#include <iterator>

namespace bricks::detail {

template <typename Range>
class enumerate_iter {
 private:
  size_t counter_{0};
  typename Range::iterator iter_;

 public:
  using difference_type = std::ptrdiff_t;
  using value_type = std::pair<size_t, typename Range::value_type>;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;

  explicit enumerate_iter(typename Range::iterator iter) : iter_{iter} {}

  auto operator++() -> enumerate_iter&
  {
    ++iter_;
    ++counter_;
    return *this;
  }

  auto operator++(int) -> enumerate_iter
  {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  auto operator==(const enumerate_iter& other) const -> bool { return iter_ == other.iter_; }
  auto operator!=(const enumerate_iter& other) const -> bool { return !(*this == other); }

  auto operator*() const -> value_type { return {counter_, *iter_}; }
};

template <typename Range>
class enumerator {
 private:
  Range& range_;

 public:
  explicit enumerator(Range& range) : range_{range} {}

  auto begin() -> enumerate_iter<Range> { return enumerate_iter<Range>{std::begin(range_)}; }
  auto end() -> enumerate_iter<Range> { return enumerate_iter<Range>{std::end(range_)}; }
};

}  // namespace bricks::detail
