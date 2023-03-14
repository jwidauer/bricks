#pragma once

#include <iterator>

namespace bricks::detail {

template <typename Range, typename UnaryPredicate>
class filter_iter {
 private:
  typename Range::iterator iter_;
  typename Range::iterator end_;
  UnaryPredicate predicate_;

  void advance()
  {
    while (iter_ != end_ && !predicate_(*iter_)) {
      ++iter_;
    }
  }

 public:
  using difference_type = std::ptrdiff_t;
  using value_type = typename Range::value_type;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::forward_iterator_tag;

  explicit filter_iter(typename Range::iterator iter, typename Range::iterator end,
                       UnaryPredicate predicate)
      : iter_{iter}, end_{end}, predicate_{predicate}
  {
    advance();
  }

  auto operator++() -> filter_iter&
  {
    ++iter_;
    advance();
    return *this;
  }

  auto operator++(int) -> filter_iter
  {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  auto operator==(const filter_iter& other) const -> bool { return iter_ == other.iter_; }
  auto operator!=(const filter_iter& other) const -> bool { return !(*this == other); }

  auto operator*() const -> value_type& { return *iter_; }
};

template <typename Range, typename UnaryPredicate>
class filterer {
 private:
  Range& range_;
  UnaryPredicate predicate_;

 public:
  explicit filterer(Range& range, UnaryPredicate predicate) : range_{range}, predicate_{predicate}
  {
  }

  auto begin() -> filter_iter<Range, UnaryPredicate>
  {
    return filter_iter<Range, UnaryPredicate>{std::begin(range_), std::end(range_), predicate_};
  }
  auto end() -> filter_iter<Range, UnaryPredicate>
  {
    return filter_iter<Range, UnaryPredicate>{std::end(range_), std::end(range_), predicate_};
  }
};

}  // namespace bricks::detail
