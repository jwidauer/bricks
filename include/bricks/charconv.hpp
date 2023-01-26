#pragma once

#include <charconv>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

namespace bricks {

/**
 * @brief Exceptionlessly convert a number to a string.
 *
 * Example:
 * ```cpp
 * std::string s;
 * if (bricks::to_string(42, s)) {
 *   std::cout << s << std::endl; // prints 42
 * }
 * ```
 *
 * @tparam T The type of number to convert.
 * @param value The number to convert.
 * @param str The string to write to.
 * @return true If the conversion was successful, false otherwise.
 */
template <typename T>
auto to_string(const T& value, std::string& str) noexcept -> bool
{
  str.clear();
  str.resize(std::numeric_limits<T>::digits10 + 1);
  auto [ptr, ec] = std::to_chars(str.data(), str.data() + str.size(), value);
  str.resize(ptr - str.data());
  return ec == std::errc();
}

/**
 * @brief Exceptionlessly convert a string to a number.
 *
 * Example:
 * ```cpp
 * int i;
 * if (bricks::from_string("42", i)) {
 *   std::cout << i << std::endl; // prints 42
 * }
 * ```
 *
 * @tparam T The type of number to convert.
 * @param str The string to convert.
 * @param value The number to write to.
 * @return true If the conversion was successful, false otherwise.
 */
template <typename T>
auto from_string(const std::string_view& str, T& value) noexcept -> bool
{
  auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
  return ec == std::errc() && ptr == str.data() + str.size();
}

}  // namespace bricks
