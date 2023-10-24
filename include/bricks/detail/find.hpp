#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "bricks/type_traits.hpp"

namespace bricks::detail {

template <class Container,
          typename std::enable_if_t<
              std::negation_v<has_find<Container, typename Container::value_type>>, bool> = true>
constexpr auto find(const Container& container, const typename Container::value_type& value)
    -> std::optional<typename Container::value_type>
{
  const auto it = std::find(std::begin(container), std::end(container), value);
  return it != std::end(container) ? std::make_optional(*it) : std::nullopt;
}

/**
 * @brief Specialization for map-like associative containers.
 */
template <class Container, typename std::enable_if_t<is_map_like_v<Container>, bool> = true>
constexpr auto find(const Container& container, const typename Container::key_type& key)
    -> std::optional<typename Container::mapped_type>
{
  const auto it = container.find(key);
  return it != std::end(container) ? std::make_optional(it->second) : std::nullopt;
}

/**
 * @brief Specialization for set-like associative containers.
 */
template <class Container, typename std::enable_if_t<is_set_like_v<Container>, bool> = true>
constexpr auto find(const Container& container, const typename Container::key_type& key)
    -> std::optional<typename Container::value_type>
{
  const auto it = container.find(key);
  return it != std::end(container) ? std::make_optional(*it) : std::nullopt;
}

/**
 * @brief Specialization for strings.
 */
template <class CharT, class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT>>
constexpr auto find(const std::basic_string<CharT, Traits, Allocator>& str, const CharT& value)
    -> std::optional<CharT>
{
  const auto pos = str.find(value);
  return pos != std::basic_string<CharT, Traits, Allocator>::npos ? std::make_optional(str[pos])
                                                                  : std::nullopt;
}

/**
 * @brief Specialization for string_views.
 */
template <class CharT, class Traits = std::char_traits<CharT>>
constexpr auto find(const std::basic_string_view<CharT, Traits>& str, const CharT& value)
    -> std::optional<CharT>
{
  const auto pos = str.find(value);
  return pos != std::basic_string_view<CharT, Traits>::npos ? std::make_optional(str[pos])
                                                            : std::nullopt;
}

}  // namespace bricks::detail
