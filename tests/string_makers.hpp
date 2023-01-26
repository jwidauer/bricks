#pragma once

#include <doctest/doctest.h>

#include <future>
#include <optional>

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

}  // namespace doctest
