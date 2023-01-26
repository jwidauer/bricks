#pragma once

#include <algorithm>
#include <string>
#include <type_traits>

#include "bricks/type_traits.hpp"

namespace bricks::detail {

/**
 * @brief Implementation of `contains`.
 */
template <class Container, typename std::enable_if_t<
                               !has_find_v<Container, typename Container::value_type>, bool> = true>
constexpr auto contains(const Container& container,
                        const typename Container::value_type& value) noexcept -> bool
{
  return std::find(std::begin(container), std::end(container), value) != std::end(container);
}

/**
 * @brief Specialization for containers that have a `find` method.
 */
template <class Container, typename std::enable_if_t<
                               has_find_v<Container, typename Container::key_type>, bool> = true>
constexpr auto contains(const Container& container,
                        const typename Container::key_type& key) noexcept -> bool
{
  return container.find(key) != std::end(container);
}

/**
 * @brief Specialization for strings.
 */
template <class CharT, class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT>>
constexpr auto contains(const std::basic_string<CharT, Traits, Allocator>& str,
                        const CharT& value) noexcept -> bool
{
  return str.find(value) != std::basic_string<CharT, Traits, Allocator>::npos;
}

}  // namespace bricks::detail
