/**
 * @file    type_traits.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    04/03/2020
 */

#ifndef ISML_TYPE_TRAITS_HPP
#define ISML_TYPE_TRAITS_HPP

#include <type_traits>

#include <utility>
#include <array>
#include <optional>

namespace isml {

template<typename T>
struct IsArray : std::false_type
{};

template<typename T, std::size_t N>
struct IsArray<std::array<T, N>> : std::true_type
{};

static_assert(IsArray<int>::value == false);
static_assert(IsArray<std::array<int, 10>>::value == true);
static_assert(IsArray<std::optional<int>>::value == false);

/// Check whether T is std::optional
template<typename T, typename = void>
struct IsOptional : std::false_type
{};

template<typename T>
struct IsOptional<std::optional<T>> : std::true_type
{};

static_assert(IsOptional<int>::value == false);
static_assert(IsOptional<std::array<int, 10>>::value == false);
static_assert(IsOptional<std::optional<int>>::value == true);

template<typename T>
struct IsPair : std::false_type
{};

template<typename T, typename U>
struct IsPair<std::pair<T, U>> : std::true_type {};

static_assert(IsPair<std::pair<int, char>>::value == true);

} // namespace isml

#endif // ISML_TYPE_TRAITS_HPP
