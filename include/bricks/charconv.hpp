#pragma once

#include <charconv>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>

#include "result.hpp"

namespace bricks {

/**
 * @brief Exceptionlessly convert a number to a string.
 *
 * This function is a wrapper around `std::to_chars` and will return the same error codes.
 *
 * Example:
 * @snippet charconv_test.cpp to_string-example
 *
 * @tparam T The type of number to convert.
 * @param value The number to convert.
 * @return result<std::string, std::errc> Result of the conversion. Will contain the error code
 *        if the conversion failed.
 */
template <typename T>
auto to_string(const T& value) noexcept -> result<std::string, std::errc>
{
  std::string str(std::numeric_limits<T>::digits10 + 1, '\0');
  auto [ptr, ec] = std::to_chars(str.data(), str.data() + str.size(), value);

  if (ec != std::errc()) {
    return ec;
  }

  str.resize(std::distance(str.data(), ptr));
  return str;
}

/**
 * @brief Exceptionlessly convert a string to a number.
 *
 * This function is a wrapper around `std::from_chars` and will return the same error codes.
 * It, however, will return `std::errc::invalid_argument` if the string contains
 * characters that are not part of the number.
 *
 * Example:
 * @snippet charconv_test.cpp from_string-example
 *
 * @tparam T The type of number to convert.
 * @param str The string to convert.
 * @return result<T, std::errc> Result of the conversion. Will contain the error code
 *       if the conversion failed.
 */
template <typename T>
auto from_string(const std::string_view& str) noexcept -> result<T, std::errc>
{
  T value;
  auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

  if (ec != std::errc()) {
    return ec;
  }

  if (ptr != str.data() + str.size()) {
    return std::errc::invalid_argument;
  }

  return value;
}

}  // namespace bricks
