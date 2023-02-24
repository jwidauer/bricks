#pragma once

#include <iterator>
#include <type_traits>

namespace bricks {

namespace detail {

template <typename T, typename U, typename = void>
struct has_find : std::false_type {
};

template <typename T, typename U>
struct has_find<T, U, std::void_t<decltype(std::declval<T>().find(std::declval<U>()))>>
    : std::true_type {
};

template <typename T, typename = void>
struct is_iterator : std::false_type {
};

template <typename T>
struct is_iterator<T, std::void_t<typename std::iterator_traits<T>::iterator_category>>
    : std::true_type {
};

}  // namespace detail

/**
 * @brief Helper variable template for static assertions.
 */
template <class>
inline constexpr bool always_false_v = false;

/**
 * @brief Checks if a type has a `find` method taking a specific type.
 *
 * Provides the member constant `value` which is `true` if the type has a `find` method taking a
 * specific type, otherwise value is equal to `false`.
 *
 * @tparam T The type to check.
 * @tparam U The type of the argument to `find`.
 */
template <typename T, typename U>
struct has_find : detail::has_find<T, U>::type {
};

/**
 * @relates has_find
 * @brief Helper variable template to check if a type has a `find` method taking a specific type.
 *
 * Example:
 * @snippet type_traits_test.cpp has_find-example
 */
template <class T, typename U>
inline constexpr bool has_find_v = has_find<T, U>::value;

/**
 * @brief Checks if a type is an iterator.
 *
 * Provides member constant `value` which is `true` if the type is an iterator, otherwise value is
 * equal to `false`.
 *
 * @tparam T The type to check.
 */
template <typename T>
struct is_iterator : detail::is_iterator<T>::type {
};

/**
 * @brief Helper variable template to check if a type is an iterator.
 *
 * Example:
 * @snippet type_traits_test.cpp is_iterator-example
 *
 * @tparam T
 */
template <typename T>
inline constexpr bool is_iterator_v = is_iterator<T>::value;

}  // namespace bricks
