#pragma once

#include <doctest/doctest.h>

#include <bricks/detail/zip.hpp>
#include <future>
#include <optional>
#include <tuple>
#include <utility>

namespace doctest {

template <typename T>
struct StringMaker<std::optional<T>> {
  static auto convert(const std::optional<T>& value) -> String
  {
    return value ? doctest::toString(*value) : String("std::nullopt");
  }
};

template <>
struct StringMaker<std::nullopt_t> {
  static auto convert(const std::nullopt_t& /* unused */) -> String { return {"std::nullopt"}; }
};

template <>
struct StringMaker<std::future_status> {
  static auto convert(const std::future_status& value) -> String
  {
    switch (value) {
      case std::future_status::ready:
        return {"std::future_status::ready"};
      case std::future_status::timeout:
        return {"std::future_status::timeout"};
      case std::future_status::deferred:
        return {"std::future_status::deferred"};
    }
    return {"unknown"};
  }
};

template <typename... Args>
struct StringMaker<std::tuple<Args...>> {
  static auto convert(const std::tuple<Args...>& value) -> String
  {
    String result = "std::tuple{";
    std::apply(
        [&result](const auto&... args) -> void {
          ((result += doctest::toString(args) + ", "), ...);
        },
        value);
    if (sizeof...(Args) > 0) {
      const auto sz = result.size() - 2;
      result = std::move(result).substr(0, sz);  // Remove the last ", "
    }
    result += "}";
    return result;
  }
};

template <typename... Args>
struct StringMaker<bricks::detail::zip_iterator<Args...>> {
  static auto convert(const bricks::detail::zip_iterator<Args...> value) -> String
  {
    return doctest::toString(*value);
  }
};

}  // namespace doctest
