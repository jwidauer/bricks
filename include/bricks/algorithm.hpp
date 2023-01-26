#pragma once

#include <functional>
#include <future>
#include <string>
#include <type_traits>
#include <vector>

#include "bricks/detail/contains.hpp"
#include "bricks/detail/index_of.hpp"

namespace bricks {

/**
 * @brief Get the keys of an associative container.
 *
 * Example:
 * ```cpp
 * std::map<std::string, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
 * auto keys = bricks::keys(map);
 * for (auto key : keys) {
 *   std::cout << key << std::endl; // prints "a", "b", "c"
 * }
 * ```
 *
 * @tparam Container The type of the container.
 * @param input_map The container.
 * @return std::vector<typename Container::key_type> The keys.
 */
template <class Container>
auto keys(const Container& input_map) -> std::vector<typename Container::key_type>
{
  std::vector<typename Container::key_type> retval;
  retval.reserve(input_map.size());
  std::transform(std::begin(input_map), std::end(input_map), std::back_inserter(retval),
                 [](auto&& pair) { return std::get<0>(std::forward<decltype(pair)>(pair)); });

  return retval;
}

/**
 * @brief Get the values of an associative container.
 *
 * Example:
 * ```cpp
 * std::map<std::string, int> map = {{"a", 1}, {"b", 2}, {"c", 3}};
 * auto values = bricks::values(map);
 * for (auto value : values) {
 *   std::cout << value << std::endl; // prints 1, 2, 3
 * }
 * ```
 *
 * @tparam Container The type of the container.
 * @param input_map The container.
 * @return std::vector<typename Container::mapped_type> The values.
 */
template <class Container>
auto values(const Container& input_map) -> std::vector<typename Container::mapped_type>
{
  std::vector<typename Container::mapped_type> retval;
  retval.reserve(input_map.size());
  std::transform(std::begin(input_map), std::end(input_map), std::back_inserter(retval),
                 [](auto&& pair) { return std::get<1>(std::forward<decltype(pair)>(pair)); });

  return retval;
}

/**
 * @brief Bind arguments to the front of a function.
 *
 * C++17 implementation of the C++20 `std::bind_front(...)` function.
 *
 * Example:
 * ```cpp
 * auto add = [](int a, int b) { return a + b; };
 * auto add_1 = bricks::bind_front(add, 1);
 * std::cout << add_1(2) << std::endl; // prints 3
 * ```
 *
 * @param f The function to bind front arguments to.
 * @param front_args The front arguments to bind.
 */
template <typename F, typename... FrontArgs>
constexpr auto bind_front(F&& f, FrontArgs&&... front_args)
{
  // front_args are copied because multiple invocations of this closure are possible
  return [captured_f = std::forward<F>(f), front_args...](auto&&... back_args) {
    return std::invoke(captured_f, front_args..., std::forward<decltype(back_args)>(back_args)...);
  };
}

/**
 * @brief Filter a container using a predicate.
 *
 * Creates a new container of type `Container`, with the elements that satisfy the predicate.
 *
 * Example:
 * ```cpp
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * auto filtered = bricks::filter(vec, [](int i) { return i % 2 == 0; });
 * for (auto i : filtered) {
 *   std::cout << i << std::endl; // prints 2, 4
 * }
 * ```
 *
 * @tparam Container The type of the container.
 * @tparam UnaryPredicate The type of the predicate.
 * @param container The container.
 * @param predicate The predicate.
 * @return Container The filtered container.
 */
template <class Container, class UnaryPredicate>
auto filter(const Container& container, const UnaryPredicate& predicate) -> Container
{
  Container retval;
  std::copy_if(std::begin(container), std::end(container), std::back_inserter(retval), predicate);
  return retval;
}

/**
 * @brief Checks whether a container contains a specific value.
 *
 * If the container has a `find` method, it will be used to check if the container contains the
 * value. Otherwise, the container will be searched using `std::find`. If `std::find` is used, the
 * container must have a `begin` and `end` method. If the container has a `find` method, the
 * container must have a `key_type` type alias. If `std::find` throws an exception, `std::terminate`
 * will be called.
 *
 * Example:
 * ```cpp
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * std::cout << std::boolalpha << bricks::contains(vec, 3) << std::endl; // prints true
 * std::cout << std::boolalpha << bricks::contains(vec, 6) << std::endl; // prints false
 * ```
 *
 * @tparam Container The type of the container.
 * @tparam Value The type of the value.
 * @param container The container.
 * @param value The value to check for.
 * @return true If the container contains the value, false otherwise.
 */
template <class Container, class Value>
constexpr auto contains(const Container& container, const Value& value) noexcept -> bool
{
  return detail::contains(container, value);
}

/**
 * @brief Checks if a container contains a value that satisfies a predicate.
 *
 * Example:
 * ```cpp
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * std::cout << std::boolalpha << bricks::contains_if(vec, [](int i) { return i % 2 == 0; }) <<
 * std::endl; // prints true
 * ```
 *
 * @tparam Container The type of the container.
 * @tparam UnaryPredicate The type of the predicate.
 * @param container The container.
 * @param predicate The predicate.
 * @return true If the container contains a value that satisfies the predicate, false otherwise.
 */
template <class Container, class UnaryPredicate>
constexpr auto contains_if(const Container& container, const UnaryPredicate& predicate) -> bool
{
  return std::find_if(std::begin(container), std::end(container), predicate) != std::end(container);
}

/**
 * @brief Get the index of the first occurence of a value in a container.
 *
 * If the container has a `find` method, it will be used to get the index of the value. Otherwise,
 * the container will be searched using `std::find`. If `std::find` is used, the container must have
 * a `begin` and `end` method. If the container has a `find` method, the container must have a
 * `key_type` type alias. If `std::find` throws an exception, `std::terminate` will be called.
 *
 * Example:
 * ```cpp
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * std::cout << bricks::index_of(vec, 3) << std::endl; // prints 2
 * ```
 *
 * @tparam Container The type of the container.
 * @tparam Value The type of value to search for.
 * @param container The container.
 * @param value The value to get the index of.
 * @return constexpr std::optional<size_t> The index of the value, if it exists.
 */
template <class Container, class Value>
constexpr auto index_of(const Container& container, const Value& value) noexcept
    -> std::optional<size_t>
{
  return detail::index_of(container, value);
}

/**
 * @brief Get the index of the first element in a container for which a predicate is true.
 *
 * Example:
 * ```cpp
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * std::cout << bricks::index_of_if(vec, [](int i) { return i % 2 == 0; }) << std::endl; // prints 1
 * ```
 *
 * @tparam Container The type of the container.
 * @tparam UnaryPredicate The type of the predicate.
 * @param container The container.
 * @param predicate The predicate.
 * @return constexpr std::optional<size_t> The index of the value, if it exists.
 */
template <class Container, class UnaryPredicate>
constexpr auto index_of_if(const Container& container, const UnaryPredicate& predicate)
    -> std::optional<size_t>
{
  const auto it = std::find_if(std::begin(container), std::end(container), predicate);
  return it != std::end(container) ? std::make_optional(std::distance(std::begin(container), it))
                                   : std::nullopt;
}

/**
 * @brief Check whether a future is ready after a timeout.
 *
 * Example:
 * @code {.cpp}
 * auto future = std::async(std::launch::async, []() {
 *   std::this_thread::sleep_for(std::chrono::seconds(1));
 *   return 42;
 * });
 * // Will print "Future is ready!" after 2 seconds.
 * if (bricks::is_ready(future, std::chrono::seconds(2))) {
 *   std::cout << "Future is ready!" << std::endl;
 * }
 * @endcode
 *
 *
 * @tparam T The type of the future.
 * @param future The future.
 * @param timeout The timeout.
 * @return true If the future is ready, false otherwise.
 */
template <class T, class Rep, class Period>
auto is_ready_after(const std::future<T>& future,
                    const std::chrono::duration<Rep, Period>& timeout) noexcept -> bool
{
  return future.wait_for(timeout) == std::future_status::ready;
}

/**
 * @brief Check whether a future is ready at a specific time.
 *
 * Example:
 * @code {.cpp}
 * auto future = std::async(std::launch::async, []() {
 *   std::this_thread::sleep_for(std::chrono::seconds(1));
 *   return 42;
 * });
 * // Will print "Future is ready!" after 2 seconds.
 * if (bricks::is_ready_at(future, std::chrono::steady_clock::now() + std::chrono::seconds(2))) {
 *   std::cout << "Future is ready!" << std::endl;
 * }
 * @endcode
 *
 * @tparam T The type of the future.
 * @param future The future.
 * @param timeout The timeout.
 * @return true If the future is ready, false otherwise.
 */
template <class T, class Clock, class Duration>
auto is_ready_at(const std::future<T>& future,
                 const std::chrono::time_point<Clock, Duration>& timeout) noexcept -> bool
{
  return future.wait_until(timeout) == std::future_status::ready;
}

}  // namespace bricks
