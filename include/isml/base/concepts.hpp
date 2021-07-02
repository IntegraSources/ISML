/**
 * @file    concepts.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    7/23/20
 */

#ifndef ISML_UTILITY_CONCEPTS_HPP
#define ISML_UTILITY_CONCEPTS_HPP

#include <cstddef>
#include <iterator>
#include <concepts>
#include <limits>

#include <isml/base/type_traits.hpp>

namespace isml {

template<typename From, typename To>
concept ConvertibleTo = std::is_convertible_v<From, To> &&
    requires(std::add_rvalue_reference_t<From>(&f)())
    {
        static_cast<To>(f());
    };

template<typename T>
concept Swappable = std::is_swappable_v<T>;

template<typename T, typename U>
concept WeaklyEqualityComparableWith =
    requires(const std::remove_reference_t<T>& t,
             const std::remove_reference_t<U>& u)
    {
        { t == u } -> ConvertibleTo<bool>;
        { t != u } -> ConvertibleTo<bool>;
        { u == t } -> ConvertibleTo<bool>;
        { u != t } -> ConvertibleTo<bool>;
    };

template<typename T, typename Allocator>
concept Erasable =
    requires(T* t, std::remove_reference_t<Allocator>& m)
    {
        { std::allocator_traits<Allocator>::destroy(m, t) } -> std::same_as<void>;
    };

template<typename T>
concept EqualityComparable = WeaklyEqualityComparableWith<T, T>;

template<
    typename C,
    typename T = typename C::value_type,
    typename allocator_type = typename C::allocator_type,
    typename reference = typename C::reference,
    typename const_reference = typename C::const_reference,
    typename iterator = typename C::iterator,
    typename const_iterator = typename C::const_iterator,
    typename difference_type = typename C::difference_type,
    typename size_type = typename C::size_type
>
concept Container =
    EqualityComparable<C> &&
    Swappable<C> &&
    requires(std::remove_reference_t<C>& c)
    {
        requires Erasable<T, allocator_type>;
        requires std::is_same_v<reference, T&>;
        requires std::is_same_v<const_reference, const T&>;
        requires std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<iterator>::iterator_category>;
        requires std::is_convertible_v<iterator, const_iterator>;
        requires std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<const_iterator>::iterator_category>;
        requires std::is_same_v<difference_type, typename std::iterator_traits<iterator>::difference_type>;
        requires std::numeric_limits<size_type>::max() >= std::numeric_limits<difference_type>::max();

        { c.begin() } -> std::same_as<iterator>;
        { std::as_const(c).begin() } -> std::same_as<const_iterator>;
        { c.end() } -> std::same_as<iterator>;
        { std::as_const(c).end() } -> std::same_as<const_iterator>;
        { c.cbegin() } -> std::same_as<const_iterator>;
        { c.cend() } -> std::same_as<const_iterator>;
        { c.size() } -> std::same_as<size_type>;
        { c.max_size() } -> std::same_as<size_type>;
        { c.empty() } -> ConvertibleTo<bool>;
    };

template<
    typename C,
    typename value_type = typename C::value_type,
    typename size_type = typename C::size_type
>
concept SequenceContainer = Container<C> &&
    requires(size_type n, value_type t)
    {
        { C(n, t) } -> std::same_as<C>;
    };

template<
    typename C,
    typename Key = typename C::key_type,
    typename value_type = typename C::value_type,
    typename iterator = typename C::iterator,
    typename const_iterator = typename C::const_iterator,
    typename size_type = typename C::size_type
>
concept AssociativeContainer = Container<C> &&
    requires(C a, value_type t, Key k)
    {
        typename C::key_type;
        { a.insert(t) } -> std::same_as<std::pair<iterator, bool>>;
        { a.erase(k) } -> std::same_as<size_type>;
        { a.find(k) } -> std::same_as<iterator>;
        { std::as_const(a).find(k) } -> std::same_as<const_iterator>;
        { a.count(k) } -> std::same_as<size_type>;
        { a.contains(k) } -> std::same_as<bool>;
    };

template<typename T, typename = void>
struct HasMappedType : std::false_type
{};

template<typename T>
struct HasMappedType<T, std::void_t<typename T::mapped_type>> : std::true_type
{};

template<typename C>
concept Map = AssociativeContainer<C> and HasMappedType<C>::value;

} // namespace isml

#endif // ISML_UTILITY_CONCEPTS_HPP
