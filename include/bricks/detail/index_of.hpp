#pragma once

#include <algorithm>
#include <optional>
#include <string>

#include "bricks/type_traits.hpp"

namespace bricks::detail {

/**
 * @brief Implementation of `index_of`.
 */
template <class Container,
          typename std::enable_if_t<
              std::negation_v<has_find<Container, typename Container::value_type>>, bool> = true>
constexpr auto index_of(const Container& container,
                        const typename Container::value_type& value) noexcept
    -> std::optional<std::size_t>
{
  const auto it = std::find(std::begin(container), std::end(container), value);
  return it != std::end(container) ? std::make_optional(std::distance(std::begin(container), it))
                                   : std::nullopt;
}

/**
 * @brief Specialization for containers that have a `find` method.
 */
template <class Container, typename std::enable_if_t<
                               has_find_v<Container, typename Container::key_type>, bool> = true>
constexpr auto index_of(const Container& container,
                        const typename Container::key_type& key) noexcept
    -> std::optional<std::size_t>
{
  const auto it = container.find(key);
  return it != std::end(container) ? std::make_optional(std::distance(std::begin(container), it))
                                   : std::nullopt;
}

/**
 * @brief Specialization for strings.
 */
template <class CharT, class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT>>
constexpr auto index_of(const std::basic_string<CharT, Traits, Allocator>& str,
                        const CharT& value) noexcept -> std::optional<std::size_t>
{
  const auto pos = str.find(value);
  return pos != std::basic_string<CharT, Traits, Allocator>::npos ? std::make_optional(pos)
                                                                  : std::nullopt;
}

/**
 * @brief Specialization for string_views.
 */
template <class CharT, class Traits = std::char_traits<CharT>>
constexpr auto index_of(const std::basic_string_view<CharT, Traits>& str,
                        const CharT& value) noexcept -> std::optional<std::size_t>
{
  const auto pos = str.find(value);
  return pos != std::basic_string_view<CharT, Traits>::npos ? std::make_optional(pos)
                                                            : std::nullopt;
}

}  // namespace bricks::detail
