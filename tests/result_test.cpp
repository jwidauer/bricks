#include <doctest/doctest.h>

#include <bricks/result.hpp>
#include <unordered_map>

TEST_SUITE_BEGIN("result");

using namespace bricks;  // NOLINT

struct value {
  int i;

  auto operator==(int rhs) const -> bool { return i == rhs; }
  auto operator!=(int rhs) const -> bool { return i != rhs; }

  auto operator==(const value& rhs) const -> bool { return i == rhs.i; }
  auto operator!=(const value& rhs) const -> bool { return i != rhs.i; }
};

template <>
struct std::hash<value> {
  [[nodiscard]] auto operator()(const value& v) const noexcept -> std::size_t
  {
    return std::hash<int>{}(v.i);
  }
};

TEST_CASE_TEMPLATE("value container", T, detail::value_container<int, detail::ok_tag>,
                   detail::value_container<value, detail::ok_tag>)
{
  SUBCASE("constructors")
  {
    T c1{typename T::value_type{42}};
    CHECK_EQ(c1.get(), 42);

    T c2{c1};
    CHECK_EQ(c2.get(), 42);

    T c3{std::move(c1)};  // NOLINT
    CHECK_EQ(c3.get(), 42);
  }

  SUBCASE("assignment")
  {
    T c1{typename T::value_type{1}};
    c1 = typename T::value_type{42};
    CHECK_EQ(c1.get(), 42);

    const T val{typename T::value_type{42}};
    T c2{typename T::value_type{1}};
    c2 = val;
    CHECK_EQ(c2.get(), 42);

    T c3{typename T::value_type{1}};
    c3 = std::move(c1);  // NOLINT
    CHECK_EQ(c3.get(), 42);
  }

  SUBCASE("get")
  {
    T c1{typename T::value_type{42}};
    CHECK_EQ(c1.get(), 42);
  }

  SUBCASE("operator ==")
  {
    T c1{typename T::value_type{42}};
    CHECK(c1 == T{typename T::value_type{42}});
  }

  SUBCASE("operator !=")
  {
    T c1{typename T::value_type{42}};
    CHECK(c1 != T{typename T::value_type{43}});
  }

  SUBCASE("hash")
  {
    std::unordered_map<T, int> map;
    map[T{typename T::value_type{42}}] = 43;
    CHECK_EQ(map[T{typename T::value_type{42}}], 43);
  }
}

TEST_CASE("Example")
{
  /// [result-example]
  // Division can fail by either dividing by zero or having a non zero remainder
  auto divide = [](int a, int b) -> bricks::result<int, std::invalid_argument> {
    if (b == 0) {
      return {std::invalid_argument{"division by zero"}};
    }
    if (a % b != 0) {
      return {std::invalid_argument{"non zero remainder"}};
    }
    return {a / b};
  };

  // By using the `bricks::result` type, you can avoid throwing exceptions for potential failures
  const auto res = divide(42, 2);
  if (res.is_value()) {
    INFO("The result is ", res.unwrap());
  } else {
    INFO("The error is: ", res.unwrap_error().what());
  }

  // It can also be used to easily chain function calls together in a functional style
  auto scale_by_two = [](int i) -> bricks::result<int, std::invalid_argument> {
    if (i == 0) return {std::invalid_argument{"scaling zero is not supported"}};
    return {i * 2};
  };

  const auto scaled_and_stringified = divide(42, 2)
                                          .and_then(scale_by_two)
                                          .map([](int i) { return std::to_string(i); })
                                          .unwrap_or("something went wrong");

  CHECK(scaled_and_stringified == "42");
  /// [result-example]
}

TEST_CASE("Construction")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK(res.is_value());
    CHECK_FALSE(res.is_error());
    CHECK_EQ(res.unwrap(), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK_FALSE(res.is_value());
    CHECK(res.is_error());
    CHECK_EQ(res.unwrap_error(), "error");
  }

  SUBCASE("value and error types are equal")
  {
    result<int, int> res{ok<int>{42}};
    CHECK(res.is_value());
    CHECK_FALSE(res.is_error());
    CHECK_EQ(res.unwrap(), 42);
  }

  SUBCASE("example")
  {
    /// [result-ctor-example]
    // With non equal value and error types you can easily construct a result
    bricks::result<int, std::string> res1{42};
    bricks::result<int, std::string> res2{"error"};

    // With equal value and error types you have to use the ok and err types
    bricks::result<int, int> res3{ok<int>{42}};
    bricks::result<int, int> res4{err<int>{0}};
    // bricks::result<int, int> res5{42}; // error
    /// [result-ctor-example]
  }
}

TEST_CASE("non default constructible")
{
  struct non_default_constructible {
    non_default_constructible() = delete;
    explicit non_default_constructible(int /* unused */) {}
  };

  SUBCASE("value")
  {
    result<non_default_constructible, int> res{non_default_constructible{42}};
    CHECK(res.is_value());
    CHECK_FALSE(res.is_error());
  }

  SUBCASE("error")
  {
    result<non_default_constructible, int> res{42};
    CHECK_FALSE(res.is_value());
    CHECK(res.is_error());
  }
}

TEST_CASE("from try or")
{
  SUBCASE("value")
  {
    const auto res = result<int, float>::from_try_or([]() { return 42; }, 0.);
    CHECK(res.is_value());
    CHECK_FALSE(res.is_error());
    CHECK_EQ(res.unwrap(), 42);
  }

  SUBCASE("error")
  {
    const auto res =
        result<int, float>::from_try_or([]() -> int { throw std::invalid_argument{"error"}; }, 0.);
    CHECK_FALSE(res.is_value());
    CHECK(res.is_error());
    CHECK_EQ(res.unwrap_error(), 0.);
  }

  SUBCASE("example")
  {
    /// [result-from-try-or-example]
    // You can use the `from_try_or` function to convert a function that can throw to a function
    // that returns a result
    auto divide = [](int a, int b) {
      if (b == 0) {
        throw std::invalid_argument{"division by zero"};
      }
      return a / b;
    };

    enum class error { division_by_zero, non_zero_remainder };

    const auto res = result<int, error>::from_try_or([divide]() { return divide(42, 2); },
                                                     error::division_by_zero);

    if (res.is_value()) {
      INFO("The result is ", res.unwrap());
    } else {
      INFO("The error is: ", res.unwrap_error());
    }
    /// [result-from-try-or-example]
  }
}

TEST_CASE("from try or default")
{
  SUBCASE("value")
  {
    const auto res = result<int, float>::from_try_or_default([]() { return 42; });
    CHECK(res.is_value());
    CHECK_FALSE(res.is_error());
    CHECK_EQ(res.unwrap(), 42);
  }

  SUBCASE("error")
  {
    const auto res = result<int, float>::from_try_or_default(
        []() -> int { throw std::invalid_argument{"error"}; });
    CHECK_FALSE(res.is_value());
    CHECK(res.is_error());
    CHECK_EQ(res.unwrap_error(), 0.);
  }

  SUBCASE("example")
  {
    /// [result-from-try-or-default-example]
    // You can use the `from_try_or_default` function to convert a function that can throw to a
    // function that returns a result
    auto divide = [](int a, int b) {
      if (b == 0) {
        throw std::invalid_argument{"division by zero"};
      }
      return a / b;
    };

    const auto res = result<int, float>::from_try_or_default([divide]() { return divide(42, 2); });

    if (res.is_value()) {
      INFO("The result is ", res.unwrap());
    } else {
      INFO("The error is: ", res.unwrap_error());  // 0
    }
    /// [result-from-try-or-default-example]
  }
}

TEST_CASE("from try or else")
{
  SUBCASE("value")
  {
    const auto res = result<int, float>::from_try_or_else([]() noexcept { return 42; },
                                                          []() noexcept { return err<float>{1.}; });
    CHECK(res.is_value());
    CHECK_FALSE(res.is_error());
    CHECK_EQ(res.unwrap(), 42);
  }

  SUBCASE("error")
  {
    const auto res = result<int, float>::from_try_or_else(
        []() -> int { throw std::invalid_argument{"error"}; }, []() { return err<float>{1.}; });
    CHECK_FALSE(res.is_value());
    CHECK(res.is_error());
    CHECK_EQ(res.unwrap_error(), 1.);
  }

  SUBCASE("error function throws")
  {
    auto f = []() {
      const auto res = result<int, float>::from_try_or_else(
          []() -> int { throw std::invalid_argument{"value error"}; },
          []() -> err<float> { throw std::invalid_argument{"error error"}; });
    };
    CHECK_THROWS_WITH_AS(f(), "error error", std::invalid_argument);
  }

  SUBCASE("example")
  {
    /// [result-from-try-or-else-example]
    // You can use the `from_try_or_else` function to convert a function that can throw to a
    // function that returns a result
    auto divide = [](int a, int b) {
      if (b == 0) {
        throw std::invalid_argument{"division by zero"};
      }
      return a / b;
    };

    enum class error { division };

    const auto res = result<int, error>::from_try_or_else(
        [divide]() { return divide(42, 2); }, []() noexcept { return error::division; });

    if (res.is_value()) {
      INFO("The result is ", res.unwrap());
    } else {
      INFO("The error is: ", res.unwrap_error());
    }
    /// [result-from-try-or-else-example]
  }
}

TEST_CASE("result from try")
{
  SUBCASE("value")
  {
    const auto res = result_from_try([]() { return 42; });
    CHECK(res.is_value());
    CHECK_FALSE(res.is_error());
    CHECK_EQ(res.unwrap(), 42);
  }

  SUBCASE("error")
  {
    const auto res = result_from_try([]() -> int { throw std::invalid_argument{"error"}; });
    CHECK_FALSE(res.is_value());
    CHECK(res.is_error());
    CHECK_THROWS_AS_MESSAGE(std::rethrow_exception(res.unwrap_error()), std::invalid_argument,
                            "error");
  }

  SUBCASE("example")
  {
    /// [result-from-try-example]
    // You can use the `result_from_try` function to convert a function that can throw an
    // exception into a function that returns a `bricks::result`
    auto divide = [](int a, int b) {
      if (b == 0) {
        throw std::invalid_argument{"division by zero"};
      }
      if (a % b != 0) {
        throw std::invalid_argument{"non zero remainder"};
      }
      return a / b;
    };

    const auto res = result_from_try([&divide]() { return divide(42, 2); });
    if (res.is_value()) {
      INFO("The result is ", res.unwrap());
    } else {
      CHECK_THROWS_AS(std::rethrow_exception(res.unwrap_error()), std::invalid_argument);
    }
    /// [result-from-try-example]
  }
}

TEST_CASE("Assignment")
{
  SUBCASE("value")
  {
    result<int, std::string> res{"error"};
    res = 42;
    CHECK(res.is_value());
    CHECK_FALSE(res.is_error());
    CHECK_EQ(res.unwrap(), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{42};
    res = "error";
    CHECK_FALSE(res.is_value());
    CHECK(res.is_error());
    CHECK_EQ(res.unwrap_error(), "error");
  }

  SUBCASE("value to error")
  {
    result<int, std::string> res{42};
    res = "error";
    CHECK_FALSE(res.is_value());
    CHECK(res.is_error());
    CHECK_EQ(res.unwrap_error(), "error");
  }

  SUBCASE("error to value")
  {
    result<int, std::string> res{"error"};
    res = 42;
    CHECK(res.is_value());
    CHECK_FALSE(res.is_error());
    CHECK_EQ(res.unwrap(), 42);
  }

  SUBCASE("equal value and error types")
  {
    result<int, int> res{err<int>{0}};
    res = ok<int>{42};
    CHECK(res.is_value());
    CHECK_FALSE(res.is_error());
    CHECK_EQ(res.unwrap(), 42);
  }

  SUBCASE("example")
  {
    /// [result-assign-example]
    bricks::result<int, std::string> res{"error"};
    CHECK(res.is_error());

    res = 42;
    CHECK(res.is_value());

    res = "error";
    CHECK(res.is_error());

    res = ok<int>{42};
    CHECK(res.is_value());

    res = err<std::string>{"error"};
    CHECK(res.is_error());
    /// [result-assign-example]
  }
}

TEST_CASE("Copy construction")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    result<int, std::string> copy{res};  // NOLINT
    CHECK(copy.is_value());
    CHECK_FALSE(copy.is_error());
    CHECK_EQ(copy.unwrap(), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    result<int, std::string> copy{res};  // NOLINT
    CHECK_FALSE(copy.is_value());
    CHECK(copy.is_error());
    CHECK_EQ(copy.unwrap_error(), "error");
  }
}

TEST_CASE("Move construction")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    result<int, std::string> copy{std::move(res)};
    CHECK(copy.is_value());
    CHECK_FALSE(copy.is_error());
    CHECK_EQ(copy.unwrap(), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    result<int, std::string> copy{std::move(res)};
    CHECK_FALSE(copy.is_value());
    CHECK(copy.is_error());
    CHECK_EQ(copy.unwrap_error(), "error");
  }
}

TEST_CASE("Copy assignment")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    result<int, std::string> copy{"error"};
    copy = res;
    CHECK(copy.is_value());
    CHECK_FALSE(copy.is_error());
    CHECK_EQ(copy.unwrap(), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    result<int, std::string> copy{42};
    copy = res;
    CHECK_FALSE(copy.is_value());
    CHECK(copy.is_error());
    CHECK_EQ(copy.unwrap_error(), "error");
  }
}

TEST_CASE("Move assignment")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    result<int, std::string> copy{"error"};
    copy = std::move(res);
    CHECK(copy.is_value());
    CHECK_FALSE(copy.is_error());
    CHECK_EQ(copy.unwrap(), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    result<int, std::string> copy{42};
    copy = std::move(res);
    CHECK_FALSE(copy.is_value());
    CHECK(copy.is_error());
    CHECK_EQ(copy.unwrap_error(), "error");
  }
}

TEST_CASE("Is value")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK(res.is_value());
    CHECK_FALSE(res.is_error());
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK_FALSE(res.is_value());
    CHECK(res.is_error());
  }

  SUBCASE("example")
  {
    /// [result-is-value-example]
    result<int, std::string> res{42};
    if (res.is_value()) {
      // do something with the value
    }
    /// [result-is-value-example]
  }
}

TEST_CASE("Is error")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK_FALSE(res.is_error());
    CHECK(res.is_value());
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK(res.is_error());
    CHECK_FALSE(res.is_value());
  }

  SUBCASE("example")
  {
    /// [result-is-error-example]
    result<int, std::string> res{"error"};
    if (res.is_error()) {
      // do something with the error
    }
    /// [result-is-error-example]
  }
}

TEST_CASE("Map")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    auto mapped = res.map([](int value) { return value * 2; });
    CHECK(mapped.is_value());
    CHECK_FALSE(mapped.is_error());
    CHECK_EQ(mapped.unwrap(), 84);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    auto mapped = res.map([](int value) { return value * 2; });
    CHECK_FALSE(mapped.is_value());
    CHECK(mapped.is_error());
    CHECK_EQ(mapped.unwrap_error(), "error");
  }

  SUBCASE("example")
  {
    /// [result-map-example]
    auto parse_int = [](const std::string& s) -> bricks::result<int, std::invalid_argument> {
      try {
        return {std::stoi(s)};
      } catch (const std::invalid_argument& e) {
        return {e};
      }
    };
    auto to_string = [](int i) { return std::to_string(i); };

    bricks::result<std::string, std::invalid_argument> res{"42"};
    CHECK(res.and_then(parse_int).map(to_string).unwrap() == "42");

    res = "not a number";
    CHECK(res.and_then(parse_int).map(to_string).is_error());
    /// [result-map-example]
  }
}

TEST_CASE("Map error")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    auto mapped = res.map_error([](const std::string& error) { return error + "2"; });
    CHECK(mapped.is_value());
    CHECK_FALSE(mapped.is_error());
    CHECK_EQ(mapped.unwrap(), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    auto mapped = res.map_error([](const std::string& error) { return error + "2"; });
    CHECK_FALSE(mapped.is_value());
    CHECK(mapped.is_error());
    CHECK_EQ(mapped.unwrap_error(), "error2");
  }

  SUBCASE("example")
  {
    /// [result-map-error-example]
    auto parse_int = [](const std::string& s) -> bricks::result<int, std::invalid_argument> {
      try {
        return {std::stoi(s)};
      } catch (const std::invalid_argument&) {
        return {std::invalid_argument{"invalid argument"}};
      }
    };
    auto error_to_string = [](const std::invalid_argument& e) -> std::string { return e.what(); };

    bricks::result<std::string, std::invalid_argument> res{"42"};
    CHECK(res.and_then(parse_int).map_error(error_to_string).unwrap() == 42);

    res = "invalid";
    CHECK(res.and_then(parse_int).map_error(error_to_string).unwrap_error() == "invalid argument");
    /// [result-map-error-example]
  }
}

TEST_CASE("And instead")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    auto mapped = res.and_instead(result<int, std::string>{84});
    CHECK(mapped.is_value());
    CHECK_FALSE(mapped.is_error());
    CHECK_EQ(mapped.unwrap(), 84);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    auto mapped = res.and_instead(result<int, std::string>{84});
    CHECK_FALSE(mapped.is_value());
    CHECK(mapped.is_error());
    CHECK_EQ(mapped.unwrap_error(), "error");
  }

  SUBCASE("example")
  {
    /// [result-and-instead-example]
    auto parse_int = [](const std::string& s) -> bricks::result<int, std::invalid_argument> {
      try {
        return {std::stoi(s)};
      } catch (const std::invalid_argument& e) {
        return {e};
      }
    };

    bricks::result<std::string, std::invalid_argument> res{"42"};
    CHECK(res.and_then(parse_int)  // result<int, std::invalid_argument>
              .and_instead(result<int, std::invalid_argument>{1})
              .unwrap() == 1);

    res = "not a number";
    CHECK(res.and_then(parse_int)  // result<int, std::invalid_argument>
              .and_instead(result<int, std::invalid_argument>{1})
              .is_error());
    /// [result-and-instead-example]
  }
}

TEST_CASE("And then")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    auto mapped = res.and_then([](int value) { return result<int, std::string>{value * 2}; });
    CHECK(mapped.is_value());
    CHECK_FALSE(mapped.is_error());
    CHECK_EQ(mapped.unwrap(), 84);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    auto mapped = res.and_then([](int value) { return result<int, std::string>{value * 2}; });
    CHECK_FALSE(mapped.is_value());
    CHECK(mapped.is_error());
    CHECK_EQ(mapped.unwrap_error(), "error");
  }

  SUBCASE("example")
  {
    /// [result-and-then-example]
    auto divide = [](int a, int b) -> bricks::result<int, std::string> {
      if (b == 0) {
        return {"division by zero"};
      }
      return {a / b};
    };
    auto scale = [](int a) -> bricks::result<int, std::string> {
      if (a == 0) {
        return {"scaling zero is not allowed"};
      }
      return {a * 2};
    };

    bricks::result<int, std::string> res{42};
    CHECK(res.and_then([&](int a) { return divide(a, 2); })  // result<int, std::string>
              .and_then(scale)                               // result<int, std::string>
              .unwrap() == 42);

    res = 0;
    CHECK(res.and_then([&](int a) { return divide(a, 2); })  // result<int, std::string>
              .and_then(scale)                               // result<int, std::string>
              .unwrap_error() == "scaling zero is not allowed");
    /// [result-and-then-example]
  }
}

TEST_CASE("Or instead")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    auto mapped = res.or_instead(result<int, std::string>{84});
    CHECK(mapped.is_value());
    CHECK_FALSE(mapped.is_error());
    CHECK_EQ(mapped.unwrap(), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    auto mapped = res.or_instead(result<int, std::string>{84});
    CHECK(mapped.is_value());
    CHECK_FALSE(mapped.is_error());
    CHECK_EQ(mapped.unwrap(), 84);
  }

  SUBCASE("example")
  {
    /// [result-or-instead-example]
    auto parse_int = [](const std::string& s) -> result<int, std::invalid_argument> {
      try {
        return {std::stoi(s)};
      } catch (const std::invalid_argument& e) {
        return {e};
      }
    };

    bricks::result<std::string, std::invalid_argument> res{"42"};
    CHECK(res.and_then(parse_int)  // result<int, std::invalid_argument>
              .or_instead(result<int, std::string>{"Couldn't parse"})
              .unwrap() == 42);

    res = "not a number";
    CHECK(res.and_then(parse_int)  // result<int, std::invalid_argument>
              .or_instead(result<int, std::string>{"Couldn't parse"})
              .unwrap_error() == "Couldn't parse");
    /// [result-or-instead-example]
  }
}

TEST_CASE("Or else")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    auto mapped =
        res.or_else([](const std::string& error) { return result<int, std::string>{error + "2"}; });
    CHECK(mapped.is_value());
    CHECK_FALSE(mapped.is_error());
    CHECK_EQ(mapped.unwrap(), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    auto mapped =
        res.or_else([](const std::string& error) { return result<int, std::string>{error + "2"}; });
    CHECK_FALSE(mapped.is_value());
    CHECK(mapped.is_error());
    CHECK_EQ(mapped.unwrap_error(), "error2");
  }

  SUBCASE("example")
  {
    /// [result-or-else-example]
    auto parse_int = [](const std::string& s) -> result<int, std::invalid_argument> {
      try {
        return {std::stoi(s)};
      } catch (const std::invalid_argument& e) {
        return {e};
      }
    };

    auto parse_invalid_argument = [](const std::invalid_argument& e) {
      return result<int, std::string>{std::string{"Couldn't parse: "} + e.what()};
    };

    bricks::result<std::string, std::invalid_argument> res{"42"};
    CHECK(res.and_then(parse_int)               // result<int, std::invalid_argument>
              .or_else(parse_invalid_argument)  // result<int, std::string>
              .unwrap() == 42);

    res = "not a number";
    CHECK(res.and_then(parse_int)               // result<int, std::invalid_argument>
              .or_else(parse_invalid_argument)  // result<int, std::string>
              .unwrap_error() == "Couldn't parse: stoi");
    /// [result-or-else-example]
  }
}

TEST_CASE("Unwrap")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK_EQ(res.unwrap(), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK_THROWS_AS([[maybe_unused]] const auto ret = res.unwrap(), bad_result_access);
  }

  SUBCASE("example")
  {
    /// [result-unwrap-example]
    bricks::result<int, std::string> res{"error"};
    CHECK_THROWS_WITH_AS([[maybe_unused]] const auto ret = res.unwrap(),
                         "Called `unwrap` on a result that is an error.",
                         bricks::bad_result_access);
    /// [result-unwrap-example]
  }
}

TEST_CASE("Unwrap error")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK_THROWS_AS([[maybe_unused]] const auto ret = res.unwrap_error(), bad_result_access);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK_EQ(res.unwrap_error(), "error");
  }

  SUBCASE("example")
  {
    /// [result-unwrap-error-example]
    bricks::result<int, std::string> res{42};
    CHECK_THROWS_WITH_AS([[maybe_unused]] const auto ret = res.unwrap_error(),
                         "Called `unwrap_error` on a result that is a value.",
                         bricks::bad_result_access);
    /// [result-unwrap-error-example]
  }
}

TEST_CASE("Unwrap or")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK_EQ(res.unwrap_or(84), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK_EQ(res.unwrap_or(84), 84);
  }

  SUBCASE("example")
  {
    /// [result-unwrap-or-example]
    bricks::result<int, std::string> res{42};
    CHECK(res.unwrap_or(0) == 42);

    res = "error";
    CHECK(res.unwrap_or(0) == 0);
    /// [result-unwrap-or-example]
  }
}

TEST_CASE("Unwrap or else")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK_EQ(res.unwrap_or_else([](const std::string& /* unused */) { return 84; }), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK_EQ(res.unwrap_or_else([](const std::string& /* unused */) { return 84; }), 84);
  }

  SUBCASE("example")
  {
    /// [result-unwrap-or-else-example]
    auto print_and_return = [](const std::string& e) {
      INFO(e);
      return 0;
    };

    bricks::result<int, std::string> res{42};
    CHECK(res.unwrap_or_else(print_and_return) == 42);

    res = "error";
    CHECK(res.unwrap_or_else(print_and_return) == 0);
    /// [result-unwrap-or-else-example]
  }
}

TEST_CASE("Expect")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK_EQ(res.expect("error"), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK_THROWS_WITH_AS([[maybe_unused]] const auto ret = res.expect("error"), "error",
                         bad_result_access);
  }

  SUBCASE("example")
  {
    /// [result-expect-example]
    bricks::result<int, std::string> res{"error"};
    CHECK_THROWS_WITH_AS([[maybe_unused]] const auto ret = res.expect("The result is an error."),
                         "The result is an error.", bricks::bad_result_access);
    /// [result-expect-example]
  }
}

TEST_CASE("Expect error")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK_THROWS_WITH_AS([[maybe_unused]] const auto ret = res.expect_error("error"), "error",
                         bad_result_access);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK_EQ(res.expect_error("error"), "error");
  }

  SUBCASE("example")
  {
    /// [result-expect-error-example]
    bricks::result<int, std::string> res{42};
    CHECK_THROWS_WITH_AS(
        [[maybe_unused]] const auto ret = res.expect_error("The result is a value."),
        "The result is a value.", bricks::bad_result_access);
    /// [result-expect-error-example]
  }
}

TEST_CASE("Unwrap or default")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK_EQ(res.unwrap_or_default(), 42);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK_EQ(res.unwrap_or_default(), 0);
  }

  SUBCASE("example")
  {
    /// [result-unwrap-or-default-example]
    bricks::result<int, std::string> res{42};
    CHECK(res.unwrap_or_default() == 42);

    res = "error";
    CHECK(res.unwrap_or_default() == 0);
    /// [result-unwrap-or-default-example]
  }
}

TEST_CASE("Map or")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK_EQ(res.map_or(168, [](int value) { return value * 2; }), 84);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK_EQ(res.map_or(168, [](int value) { return value * 2; }), 168);
  }

  SUBCASE("example")
  {
    /// [result-map-or-example]
    bricks::result<int, std::string> res{42};
    CHECK(res.map_or(0, [](int value) { return value * 2; }) == 84);

    res = "error";
    CHECK(res.map_or(0, [](int value) { return value * 2; }) == 0);
    /// [result-map-or-example]
  }
}

TEST_CASE("Map or else")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK_EQ(res.map_or_else([](const std::string& /* unused */) { return 84; },
                             [](int value) { return value * 4; }),
             168);
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK_EQ(res.map_or_else([](const std::string& /* unused */) { return 84; },
                             [](int value) { return value * 2; }),
             84);
  }

  SUBCASE("example")
  {
    /// [result-map-or-else-example]
    auto print_and_return = [](const std::string& e) {
      INFO(e);
      return 0;
    };

    bricks::result<int, std::string> res{42};
    CHECK(res.map_or_else(print_and_return, [](int value) { return value * 2; }) == 84);

    res = "error";
    CHECK(res.map_or_else(print_and_return, [](int value) { return value * 2; }) == 0);
    /// [result-map-or-else-example]
  }
}

TEST_CASE("Operator ==")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK(res == result<int, std::string>{42});
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK(res == result<int, std::string>{"error"});
  }

  SUBCASE("example")
  {
    /// [result-operator-equal-example]
    bricks::result<int, std::string> res{42};
    CHECK(res == bricks::result<int, std::string>{42});
    /// [result-operator-equal-example]
  }
}

TEST_CASE("Operator !=")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    CHECK(res != result<int, std::string>{84});
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    CHECK(res != result<int, std::string>{"another error"});
  }

  SUBCASE("example")
  {
    /// [result-operator-not-equal-example]
    bricks::result<int, std::string> res{42};
    CHECK(res != bricks::result<int, std::string>{84});
    /// [result-operator-not-equal-example]
  }
}

TEST_CASE("Swap")
{
  SUBCASE("value")
  {
    result<int, std::string> res{42};
    result<int, std::string> other{84};
    swap(res, other);
    CHECK(res == result<int, std::string>{84});
    CHECK(other == result<int, std::string>{42});
  }

  SUBCASE("error")
  {
    result<int, std::string> res{"error"};
    result<int, std::string> other{"another error"};
    swap(res, other);
    CHECK(res == result<int, std::string>{"another error"});
    CHECK(other == result<int, std::string>{"error"});
  }

  SUBCASE("example")
  {
    /// [result-swap-example]
    bricks::result<int, std::string> res{42};
    bricks::result<int, std::string> other{84};

    swap(res, other);

    CHECK(res == bricks::result<int, std::string>{84});
    CHECK(other == bricks::result<int, std::string>{42});
    /// [result-swap-example]
  }
}

TEST_CASE("Hash")
{
  SUBCASE("example")
  {
    /// [result-hash-example]
    std::unordered_map<bricks::result<int, std::string>, std::string> map;
    map[bricks::result<int, std::string>{42}] = "value";
    map[bricks::result<int, std::string>{"error"}] = "error";

    CHECK(map[bricks::result<int, std::string>{42}] == "value");
    CHECK(map[bricks::result<int, std::string>{"error"}] == "error");
    /// [result-hash-example]
  }
}

TEST_SUITE_END();
