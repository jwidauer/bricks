#pragma once

#include <iterator>
#include <type_traits>

namespace bricks {

namespace detail {

template <class T, class U, class = void>
struct has_find : std::false_type {
};

template <class T, class U>
struct has_find<T, U, std::void_t<decltype(std::declval<T>().find(std::declval<U>()))>>
    : std::true_type {
};

template <class T, class = void>
struct has_key_type : std::false_type {
};

template <class T>
struct has_key_type<T, std::void_t<typename T::key_type>> : std::true_type {
};

template <class T, class = void>
struct has_mapped_type : std::false_type {
};

template <class T>
struct has_mapped_type<T, std::void_t<typename T::mapped_type>> : std::true_type {
};

template <class T, class = void>
struct is_map_like : std::false_type {
};

template <class T>
struct is_map_like<T,
                   typename std::enable_if_t<std::conjunction_v<has_key_type<T>, has_mapped_type<T>,
                                                                has_find<T, typename T::key_type>>,
                                             void>> : std::true_type {
};

template <class T, class = void>
struct is_set_like : std::false_type {
};

template <class T>
struct is_set_like<T, typename std::enable_if_t<
                          std::conjunction_v<has_key_type<T>, std::negation<has_mapped_type<T>>,
                                             has_find<T, typename T::key_type>>,
                          void>> : std::true_type {
};

template <class T, class = void>
struct is_iterator : std::false_type {
};

template <class T>
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
template <class T, class U>
struct has_find : detail::has_find<T, U>::type {
};

/**
 * @relates has_find
 * @brief Helper variable template to check if a type has a `find` method taking a specific type.
 *
 * Example:
 * @snippet type_traits_test.cpp has_find-example
 */
template <class T, class U>
inline constexpr bool has_find_v = has_find<T, U>::value;

/**
 * @brief Checks if a type has a `mapped_type` member type.
 *
 * Provides the member constant `value` which is `true` if the type has a `mapped_type` member type,
 * otherwise value is equal to `false`.
 *
 * @tparam T The type to check.
 */
template <class T>
struct has_mapped_type : detail::has_mapped_type<T>::type {
};

/**
 * @relates has_mapped_type
 * @brief Helper variable template to check if a type has a `mapped_type` member type.
 *
 * Example:
 * @snippet type_traits_test.cpp has_mapped_type-example
 */
template <class T>
inline constexpr bool has_mapped_type_v = has_mapped_type<T>::value;

/**
 * @brief Checks if a type has a `key_type` member type.
 *
 * Provides the member constant `value` which is `true` if the type has a `key_type` member type,
 * otherwise value is equal to `false`.
 *
 * @tparam T The type to check.
 */
template <class T>
struct has_key_type : detail::has_key_type<T>::type {
};

/**
 * @relates has_key_type
 * @brief Helper variable template to check if a type has a `key_type` member type.
 *
 * Example:
 * @snippet type_traits_test.cpp has_key_type-example
 */
template <class T>
inline constexpr bool has_key_type_v = has_key_type<T>::value;

/**
 * @brief Checks if a type is map-like.
 *
 * Provides the member constant `value` which is `true` if the type is map-like, otherwise value is
 * equal to `false`.
 * A type is considered map-like if it has a `find` method taking a specific type and has both a
 * `key_type` and a `mapped_type` member type.
 *
 * @tparam T The type to check.
 */
template <class T>
struct is_map_like : detail::is_map_like<T> {
};

/**
 * @relates is_map_like
 * @brief Helper variable template to check if a type is map-like.
 *
 * Example:
 * @snippet type_traits_test.cpp is_map_like-example
 */
template <class T>
inline constexpr bool is_map_like_v = is_map_like<T>::value;

/**
 * @brief Checks if a type is set-like.
 *
 * Provides the member constant `value` which is `true` if the type is set-like, otherwise value is
 * equal to `false`.
 * A type is considered set-like if it has a `find` method taking a specific type and has a
 * `key_type` member type but no `mapped_type` member type.
 *
 * @tparam T The type to check.
 */
template <class T>
struct is_set_like : detail::is_set_like<T> {
};

/**
 * @relates is_set_like
 * @brief Helper variable template to check if a type is set-like.
 *
 * Example:
 * @snippet type_traits_test.cpp is_set_like-example
 */
template <class T>
inline constexpr bool is_set_like_v = is_set_like<T>::value;

/**
 * @brief Checks if a type is an iterator.
 *
 * Provides member constant `value` which is `true` if the type is an iterator, otherwise value is
 * equal to `false`.
 *
 * @tparam T The type to check.
 */
template <class T>
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
template <class T>
inline constexpr bool is_iterator_v = is_iterator<T>::value;

}  // namespace bricks
