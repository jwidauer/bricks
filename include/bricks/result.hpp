#pragma once

#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <variant>

#include "type_traits.hpp"

namespace bricks {

/**
 * @brief This is the type of the error thrown when accessing a bad result.
 *
 * @related result
 *
 * It is thrown in the following cases:
 * - When accessing the value of an error result.
 * - When accessing the error of a value result.
 *
 */
class bad_result_access : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

/**
 * @brief A class to hold a value.
 *
 * @related result
 */
template <typename T>
class ok {
 public:
  ok() = default;
  ok(const ok&) = default;
  auto operator=(const ok&) -> ok& = default;
  ok(ok&&) noexcept = default;
  auto operator=(ok&&) noexcept -> ok& = default;
  ~ok() = default;

  explicit ok(T value) noexcept(std::is_nothrow_move_constructible_v<T>) : value_(std::move(value))
  {
  }
  auto operator=(T value) noexcept(std::is_nothrow_move_assignable_v<T>) -> ok&
  {
    value_ = std::move(value);
    return *this;
  }

  [[nodiscard]] constexpr auto get() noexcept -> T& { return value_; }
  [[nodiscard]] constexpr auto get() const noexcept -> const T& { return value_; }

  [[nodiscard]] constexpr auto operator==(const ok& other) const noexcept -> bool
  {
    return value_ == other.value_;
  }
  [[nodiscard]] constexpr auto operator!=(const ok& other) const noexcept -> bool
  {
    return !(*this == other);
  }

 private:
  T value_;
};

/**
 * @brief A class to hold an error value.
 *
 * @related result
 */
template <typename E>
class err {
 public:
  err() = default;
  err(const err&) = default;
  auto operator=(const err&) -> err& = default;
  err(err&&) noexcept = default;
  auto operator=(err&&) noexcept -> err& = default;
  ~err() = default;

  explicit err(E error) noexcept(std::is_nothrow_move_constructible_v<E>) : error_(std::move(error))
  {
  }
  auto operator=(E error) noexcept(std::is_nothrow_move_assignable_v<E>) -> err&
  {
    error_ = std::move(error);
    return *this;
  }

  [[nodiscard]] constexpr auto get() noexcept -> E& { return error_; }
  [[nodiscard]] constexpr auto get() const noexcept -> const E& { return error_; }

  [[nodiscard]] constexpr auto operator==(const err& other) const noexcept -> bool
  {
    return error_ == other.error_;
  }
  [[nodiscard]] constexpr auto operator!=(const err& other) const noexcept -> bool
  {
    return !(*this == other);
  }

 private:
  E error_;
};

/**
 * @brief A class to represent the result of an operation.
 *
 * This class is used to represent the result of an operation, which can fail. It can either be a
 * value or an error. It is similar to the `std::expected` proposal or the rust `Result` type.
 *
 * The value type and the error type can be different. If they are the same, one can use the `ok<T>`
 * and `err<E>` types to construct and assign the result.
 *
 * Example:
 * @snippet result_test.cpp result-example
 */
template <typename T, typename E>
class result {
  using variant_t = std::variant<ok<T>, err<E>>;

 public:
  /** @brief The value type of the result. */
  using value_type = T;
  /** @brief The error type of the result. */
  using error_type = E;

  result() = delete;

  result(const result&) noexcept(std::is_nothrow_copy_constructible_v<variant_t>) = default;
  auto operator=(const result&) noexcept(std::is_nothrow_copy_assignable_v<variant_t>)
      -> result& = default;

  result(result&&) noexcept = default;
  auto operator=(result&&) noexcept -> result& = default;

  ~result() = default;

  /**
   * @brief Construct a new result object from a value.
   *
   * If the value and error types are the same, one must use the `ok<T>` and `err<E>` types to
   * construct the result.
   * Otherwise the value type must be convertible to the value type of the result.
   * And the error type must be convertible to the error type of the result.
   *
   * Example:
   * @snippet result_test.cpp result-ctor-example
   *
   * @param in The value to construct the result from.
   */
  template <typename U, typename std::enable_if_t<
                            std::is_same_v<U, ok<T>> || std::is_same_v<U, err<E>>, bool> = true>
  // cppcheck-suppress noExplicitConstructor
  constexpr result(U in) noexcept(std::is_nothrow_move_constructible_v<U>)  // NOLINT
      : value_(std::move(in))
  {
  }

  template <typename U = T, typename std::enable_if_t<!std::is_same_v<U, E>, bool> = true>
  // cppcheck-suppress noExplicitConstructor
  constexpr result(value_type in) noexcept(std::is_nothrow_move_constructible_v<ok<T>>)  // NOLINT
      : value_(ok<T>{std::move(in)})
  {
  }

  template <typename U = E, typename std::enable_if_t<!std::is_same_v<T, U>, bool> = true>
  // cppcheck-suppress noExplicitConstructor
  constexpr result(error_type in) noexcept(std::is_nothrow_move_constructible_v<err<E>>)  // NOLINT
      : value_(err<E>{std::move(in)})
  {
  }

  /**
   * @brief Assign a value to the result.
   *
   * If the value and error types are the same, one must use the `ok<T>` and `err<E>` types for the
   * assignment.
   * Otherwise the value type must be convertible to the value type of the result.
   * And the error type must be convertible to the error type of the result.
   *
   * Example:
   * @snippet result_test.cpp result-assign-example
   */
  template <typename U>
  constexpr auto operator=(U in) noexcept -> result&
  {
    using param_t = std::decay_t<U>;
    if constexpr (std::is_same_v<param_t, ok<T>> || std::is_same_v<param_t, err<E>>) {
      value_ = std::move(in);
    } else if constexpr (std::is_same_v<param_t, T> || std::is_convertible_v<param_t, T>) {
      value_ = ok<T>{std::move(in)};
    } else if constexpr (std::is_same_v<param_t, E> || std::is_convertible_v<param_t, E>) {
      value_ = err<E>{std::move(in)};
    } else if constexpr (std::is_same_v<T, E>) {
      static_assert(always_false_v<U>,
                    "Since the value and error types are the same, use ok<T> or "
                    "err<E> instead of T or E for the assignment.");
    } else {
      static_assert(always_false_v<U>, "Unsupported type for the assignment.");
    }
    return *this;
  }

  /**
   * @brief Check if the result is a value.
   *
   * Example:
   * @snippet result_test.cpp result-is-value-example
   *
   * @return true If the result is a value.
   * @return false If the result is an error.
   */
  [[nodiscard]] constexpr auto is_value() const noexcept -> bool
  {
    return std::holds_alternative<ok<T>>(value_);
  }

  /**
   * @brief Check if the result is an error.
   *
   * Example:
   * @snippet result_test.cpp result-is-error-example
   *
   * @return true If the result is an error.
   * @return false If the result is a value.
   */
  [[nodiscard]] constexpr auto is_error() const noexcept -> bool
  {
    return std::holds_alternative<err<E>>(value_);
  }

  /**
   * @brief Returns the value of the result.
   *
   * Throws a `bad_result_access` if the result is an error, with the provided message.
   *
   * Example:
   * @snippet result_test.cpp result-expect-example
   *
   * @param msg The message to use in the exception.
   * @return value_type The value of the result.
   */
  [[nodiscard]] constexpr auto expect(const std::string& msg) const -> value_type
  {
    if (is_error()) {
      throw bad_result_access{msg};
    }
    return std::get<ok<T>>(value_).get();
  }

  /**
   * @brief Returns the value of the result.
   *
   * Throws a `bad_result_access` if the result is an error.
   *
   * Example:
   * @snippet result_test.cpp result-unwrap-example
   *
   * @return value_type The value of the result.
   */
  [[nodiscard]] constexpr auto unwrap() const -> value_type
  {
    return expect("Called `unwrap` on a result that is an error.");
  }

  /**
   * @brief Returns the error of the result.
   *
   * Throws a `bad_result_access` if the result is a value, with the provided message.
   *
   * Example:
   * @snippet result_test.cpp result-expect-error-example
   *
   * @param msg The message to use in the exception.
   * @return error_type The error of the result.
   */
  [[nodiscard]] constexpr auto expect_error(const std::string& msg) const -> error_type
  {
    if (is_value()) {
      throw bad_result_access{msg};
    }
    return std::get<err<E>>(value_).get();
  }

  /**
   * @brief Returns the error of the result.
   *
   * Throws a `bad_result_access` if the result is a value.
   *
   * Example:
   * @snippet result_test.cpp result-unwrap-error-example
   *
   * @return error_type The error of the result.
   */
  [[nodiscard]] constexpr auto unwrap_error() const -> error_type
  {
    return expect_error("Called `unwrap_error` on a result that is a value.");
  }

  /**
   * @brief Returns the value of the result.
   *
   * If the result is a value, returns the value. If the result is an error, returns the provided
   * default value.
   *
   * Example:
   * @snippet result_test.cpp result-unwrap-or-example
   *
   * @param default_value The default value to return if the result is an error.
   * @return value_type The value of the result.
   */
  [[nodiscard]] constexpr auto unwrap_or(value_type default_value) const noexcept -> value_type
  {
    if (is_error()) {
      return default_value;
    }
    return std::get<ok<T>>(value_).get();
  }

  /**
   * @brief Returns the value of the result or a default constructed value.
   *
   * If the result is a value, returns the value. If the result is an error, returns the default
   * constructed value.
   *
   * Example:
   * @snippet result_test.cpp result-unwrap-or-default-example
   *
   * @return value_type The value of the result.
   */
  [[nodiscard]] constexpr auto unwrap_or_default() const noexcept -> value_type
  {
    static_assert(std::is_default_constructible_v<value_type>,
                  "The value type must be default constructible.");

    return unwrap_or(value_type{});
  }

  /**
   * @brief Returns the value of the result or the result of a function.
   *
   * If the result is a value, returns the value. If the result is an error, returns the result of
   * calling the provided function.
   *
   * Example:
   * @snippet result_test.cpp result-unwrap-or-else-example
   *
   * @param f The function to call if the result is an error. The function must take a single
   * parameter of type `error_type` and return a value of type `value_type`.
   * @return value_type The value of the result.
   */
  template <typename F>
  [[nodiscard]] constexpr auto unwrap_or_else(F&& f) const -> value_type
  {
    if (is_error()) {
      return f(std::get<err<E>>(value_).get());
    }
    return std::get<ok<T>>(value_).get();
  }

  /**
   * @brief Maps a `result<T, E>` to `result<U, E>` by applying a function to a contained value.
   *
   * This function can be used to compose the results of two functions.
   *
   * Example:
   * @snippet result_test.cpp result-map-example
   *
   * @tparam F The type of the function to apply.
   * @param f The function to apply.
   * @return result<std::invoke_result_t<F, value_type>, error_type> The result of the function.
   */
  template <typename F>
  [[nodiscard]] constexpr auto map(F&& f) const
      -> result<std::invoke_result_t<F, value_type>, error_type>
  {
    if (is_error()) {
      return {std::get<err<E>>(value_).get()};
    }
    return {ok{f(std::get<ok<T>>(value_).get())}};
  }

  /**
   * @brief Maps a `result<T, E>` to `result<T, F>` by applying a function to a contained error.
   *
   * This function can be used to pass through a successful result while handling an error.
   *
   * Example:
   * @snippet result_test.cpp result-map-error-example
   *
   * @tparam F The type of the function to apply.
   * @param f The function to apply.
   * @return result<value_type, std::invoke_result_t<F, error_type>> The result of the function.
   */
  template <typename F>
  [[nodiscard]] constexpr auto map_error(F&& f) const
      -> result<value_type, std::invoke_result_t<F, error_type>>
  {
    if (is_value()) {
      return {std::get<ok<T>>(value_).get()};
    }
    return {err{f(std::get<err<E>>(value_).get())}};
  }

  /**
   * @brief Returns the provided default (if an error) or applies a function to the contained value.
   *
   * Example:
   * @snippet result_test.cpp result-map-or-example
   *
   * @param default_value The default value to return if the result is an error.
   * @param f The function to apply if the result is a value.
   * @return std::invoke_result_t<F, value_type> The result of the function.
   */
  template <typename F>
  [[nodiscard]] constexpr auto map_or(std::invoke_result_t<F, value_type> default_value,
                                      F&& f) const -> std::invoke_result_t<F, value_type>
  {
    if (is_error()) {
      return default_value;
    }
    return f(std::get<ok<T>>(value_).get());
  }

  /**
   * @brief Maps the `result<T, E>` to `U` by applying fallback function `default_f` to a contained
   * error, or function `f` to a contained value.
   *
   * This function can be used to unpack a successful result while handling an error.
   *
   * Example:
   * @snippet result_test.cpp result-map-or-else-example
   *
   * @param default_f The fallback function to apply if the result is an error.
   * @param f The function to apply if the result is a value.
   * @return std::invoke_result_t<F, value_type> The result of the function.
   */
  template <typename F, typename G>
  [[nodiscard]] constexpr auto map_or_else(G&& default_f, F&& f) const
      -> std::invoke_result_t<F, value_type>
  {
    if (is_error()) {
      return default_f(std::get<err<E>>(value_).get());
    }
    return f(std::get<ok<T>>(value_).get());
  }

  /**
   * @brief Returns `res` if the result is a value, otherwise returns the error.
   *
   * This function can be used for control flow based on result values.
   *
   * Example:
   * @snippet result_test.cpp result-and-instead-example
   *
   * @param res The result to return if the result is a value.
   * @return result<U, error_type> The result of the function.
   */
  template <typename U>
  [[nodiscard]] constexpr auto and_instead(const result<U, error_type>& res) const
      -> result<U, error_type>
  {
    if (is_error()) {
      return {std::get<err<E>>(value_).get()};
    }
    return {res};
  }

  /**
   * @brief Calls `op` if the result is a value, otherwise returns the error.
   *
   * This function can be used for control flow based on result values.
   *
   * Example:
   * @snippet result_test.cpp result-and-then-example
   *
   * @param op The operation to perform on the value.
   * @return result<U, error_type>
   */
  template <typename Op>
  [[nodiscard]] constexpr auto and_then(Op&& op) const
      -> result<typename std::invoke_result_t<Op, value_type>::value_type, error_type>
  {
    if (is_error()) {
      return {std::get<err<E>>(value_).get()};
    }
    return {op(std::get<ok<T>>(value_).get())};
  }

  /**
   * @brief Returns `res` if the result is an error, otherwise returns the value.
   *
   * This function can be used for control flow based on result values.
   *
   * Example:
   * @snippet result_test.cpp result-or-instead-example
   *
   * @param res The result to return if the result is an error.
   * @return result<value_type, F> The result of the function.
   */
  template <typename F>
  [[nodiscard]] constexpr auto or_instead(const result<value_type, F>& res) const
      -> result<value_type, F>
  {
    if (is_value()) {
      return {std::get<ok<T>>(value_).get()};
    }
    return {res};
  }

  /**
   * @brief Calls `op` if the result is an error, otherwise returns the value.
   *
   * This function can be used for control flow based on result values.
   *
   * Example:
   * @snippet result_test.cpp result-or-else-example
   *
   * @param op The operation to perform on the error.
   * @return result<value_type, F>
   */
  template <typename Op>
  [[nodiscard]] constexpr auto or_else(Op&& op) const
      -> result<value_type, typename std::invoke_result_t<Op, error_type>::error_type>
  {
    if (is_value()) {
      return {std::get<ok<T>>(value_).get()};
    }
    return {op(std::get<err<E>>(value_).get())};
  }

  [[nodiscard]] constexpr auto operator==(const result& other) const noexcept -> bool
  {
    return value_ == other.value_;
  }
  [[nodiscard]] constexpr auto operator!=(const result& other) const noexcept -> bool
  {
    return !(*this == other);
  }

  friend constexpr auto std::hash<result>::operator()(const result& r) const noexcept
      -> std::size_t;

 private:
  variant_t value_;
};

}  // namespace bricks

/**
 * @brief Template specialization of `std::hash` for `result<T, E>`.
 * @relates result
 *
 * Example:
 * @snippet result_test.cpp result-hash-example
 */
template <typename T, typename E>
struct std::hash<bricks::result<T, E>> {
  [[nodiscard]] constexpr auto operator()(const bricks::result<T, E>& r) const noexcept
      -> std::size_t
  {
    return hash<std::variant<bricks::ok<T>, bricks::err<E>>>{}(r.value_);
  }
};

/**
 * @brief Template specialization of `std::hash` for `ok<T>`.
 * @relates ok
 */
template <typename T>
struct std::hash<bricks::ok<T>> {
  [[nodiscard]] constexpr auto operator()(const bricks::ok<T>& r) const noexcept -> std::size_t
  {
    return hash<T>{}(r.get());
  }
};

/**
 * @brief Template specialization of `std::hash` for `err<E>`.
 * @relates err
 */
template <typename E>
struct std::hash<bricks::err<E>> {
  [[nodiscard]] constexpr auto operator()(const bricks::err<E>& r) const noexcept -> std::size_t
  {
    return hash<E>{}(r.get());
  }
};
