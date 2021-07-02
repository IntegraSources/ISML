/**
 * @file    maybe.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    01.03.2021
 */

#ifndef ISML_MAYBE_HPP
#define ISML_MAYBE_HPP

#include <variant>
#include <utility>
#include <memory>       // std::addressof
#include <functional>

#include <isml/exceptions.hpp>
#include <isml/base/base_types.hpp>
#include <isml/base/type_traits.hpp>

namespace isml {

template<typename T>
class Maybe;

constexpr auto operator==(const None&, const None&) noexcept -> bool { return true; }

template<>
class Maybe<None>
{
public:
    Maybe() = delete;
};

// Traits
template<typename T>
struct IsMaybe : std::false_type
{};

template<typename T>
struct IsMaybe<Maybe<T>> : std::true_type
{};

// Functional extensions for Maybe<T>
template<typename T, typename F>
requires std::is_invocable_v<F, T>
constexpr auto map(F&& fn, Maybe<T>& maybe)
noexcept(std::is_nothrow_invocable_v<F, T>) -> Maybe<std::invoke_result_t<F, T>>;

template<typename T, typename F>
requires IsMaybe<std::invoke_result_t<F, T>>::value
constexpr auto bind(F&& fn, Maybe<T>& maybe)
noexcept(std::is_nothrow_invocable_v<F, T>) -> decltype(auto);

template<typename T>
class Maybe
{
public:
    // Construction/Destruction
    Maybe() = default;

    constexpr Maybe(None) noexcept;

    constexpr Maybe(const Maybe& other) noexcept(std::is_nothrow_copy_constructible_v<T>)
        requires std::is_copy_constructible_v<T>;

    constexpr Maybe(Maybe&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires std::is_move_constructible_v<T>;

    constexpr Maybe(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
        requires std::is_copy_constructible_v<T>;

    constexpr Maybe(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires std::is_move_constructible_v<T>;

    ~Maybe() = default;

    // Mutations
    constexpr auto operator=(None) noexcept -> Maybe&;

    constexpr auto operator=(const Maybe& other) noexcept(std::is_nothrow_copy_assignable_v<T>) -> Maybe&
        requires std::is_copy_assignable_v<T>;

    constexpr auto operator=(Maybe&& other) noexcept(std::is_nothrow_move_assignable_v<T>) -> Maybe&
        requires std::is_move_assignable_v<T>;

    template<typename U>
        requires (!IsMaybe<std::remove_cvref_t<U>>::value)
    constexpr auto operator=(U&& value) -> Maybe&;

    // Observers
    constexpr auto isSome() const noexcept -> bool;

    constexpr auto isNone() const noexcept -> bool;

    constexpr explicit operator bool() const noexcept;

    constexpr auto operator*() -> T&;

    constexpr auto operator*() const -> const T&;

    constexpr auto value() -> T&;

    constexpr auto value() const -> const T&;

    // Comparison
    friend constexpr auto operator==(const Maybe& lhs, const Maybe& rhs) -> bool
    {
        return lhs.m_var == rhs.m_var;
    }

    template<typename U>
    friend constexpr auto operator==(const Maybe& lhs, const U& rhs) -> bool
    {
        return static_cast<bool>(lhs)
               ? lhs.value() == rhs
               : false;
    }

    friend constexpr auto operator==(const Maybe& lhs, const None&) noexcept -> bool
    {
        return lhs.isNone();
    }

protected:
    std::variant<None, T> m_var {};
};

static_assert(IsMaybe<Maybe<int>>::value);
static_assert(IsMaybe<Maybe<const int&>>::value);

template<typename T>
constexpr Maybe<T>::Maybe(None) noexcept
{}

template<typename T>
constexpr Maybe<T>::Maybe(const Maybe& other) noexcept(std::is_nothrow_copy_constructible_v<T>)
    requires std::is_copy_constructible_v<T>
    : m_var(other.m_var)
{}

template<typename T>
constexpr Maybe<T>::Maybe(Maybe&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
    requires std::is_move_constructible_v<T>
    : m_var(std::move(other.m_var))
{}

template<typename T>
constexpr Maybe<T>::Maybe(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
    requires std::is_copy_constructible_v<T>
    : m_var(value)
{}

template<typename T>
constexpr Maybe<T>::Maybe(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
    requires std::is_move_constructible_v<T>
    : m_var(std::move(value))
{}

template<typename T>
constexpr auto Maybe<T>::operator=(None) noexcept -> Maybe&
{
    m_var = None();
    return *this;
}

template<typename T>
constexpr auto Maybe<T>::operator=(const Maybe& other)
noexcept(std::is_nothrow_copy_assignable_v<T>) -> Maybe&
requires std::is_copy_assignable_v<T>
{
    if (this != &other)
    {
        m_var = other.m_var;
    }

    return *this;
}

template<typename T>
constexpr auto Maybe<T>::operator=(Maybe&& other)
noexcept(std::is_nothrow_move_assignable_v<T>) -> Maybe&
requires std::is_move_assignable_v<T>
{
    if (this != &other)
    {
        m_var = std::move(other.m_var);
    }

    return *this;
}

template<typename T>
template<typename U>
requires (!IsMaybe<std::remove_cvref_t<U>>::value)
constexpr auto Maybe<T>::operator=(U&& value) -> Maybe&
{
    m_var = std::forward<U>(value);
    return *this;
}

template<typename T>
constexpr auto Maybe<T>::isSome() const noexcept -> bool
{
    return std::holds_alternative<T>(m_var);
}

template<typename T>
constexpr auto Maybe<T>::isNone() const noexcept -> bool
{
    return std::holds_alternative<None>(m_var);
}

template<typename T>
constexpr Maybe<T>::operator bool() const noexcept
{
    return std::holds_alternative<T>(m_var);
}

template<typename T>
constexpr auto Maybe<T>::operator*() -> T&
{
    if (isNone())
    {
        throw InvalidOperationException("Value is not present");
    }

    return std::get<T>(m_var);
}

template<typename T>
constexpr auto Maybe<T>::operator*() const -> const T&
{
    if (isNone())
    {
        throw InvalidOperationException("Value is not present");
    }

    return std::get<T>(m_var);
}

template<typename T>
constexpr auto Maybe<T>::value() -> T&
{
    if (isNone())
    {
        throw InvalidOperationException("Value is not present");
    }

    return std::get<T>(m_var);
}

template<typename T>
constexpr auto Maybe<T>::value() const -> const T&
{
    if (isNone())
    {
        throw InvalidOperationException("Value is not present");
    }

    return std::get<T>(m_var);
}

template<typename T, typename F>
requires std::is_invocable_v<F, T>
constexpr auto map(F&& fn, Maybe<T>& maybe)
noexcept(std::is_nothrow_invocable_v<F, T>) -> Maybe<std::invoke_result_t<F, T>>
{
    return maybe.isSome()
           ? Maybe<std::invoke_result_t<F, T>>(std::invoke(std::forward<F>(fn), *maybe))
           : Maybe<std::invoke_result_t<F, T>>();
}

template<typename T, typename F>
requires IsMaybe<std::invoke_result_t<F, T>>::value
constexpr auto bind(F&& fn, Maybe<T>& maybe)
noexcept(std::is_nothrow_invocable_v<F, T>) -> decltype(auto)
{
    return maybe.isSome()
           ? std::invoke(std::forward<F>(fn), *maybe)
           : std::invoke_result_t<F, T>{};
}

/**
 * @class   Maybe<T&>
 * @brief   An specialization of the Maybe class template for reference types.
 * @tparam  T  Value type.
 */

template<typename T>
class Maybe<T&>
{
public:
    // Construction/Destruction
    Maybe() = default;

    constexpr Maybe(None) noexcept;

    constexpr Maybe(const Maybe& other) noexcept;

    constexpr Maybe(Maybe&& other) noexcept;

    template<typename U>
    requires (!IsMaybe<std::remove_cvref_t<U>>::value)
    constexpr explicit Maybe(U&& ref) noexcept;

    ~Maybe() = default;

    // Mutations
    constexpr auto operator=(None) noexcept -> Maybe&;

    constexpr auto operator=(const Maybe& other) noexcept -> Maybe&;

    constexpr auto operator=(Maybe&& other) noexcept -> Maybe&;

    template<typename U>
    requires (!IsMaybe<std::remove_cvref_t<U>>::value)
    constexpr auto operator=(U&& value) -> Maybe&;

    // Observers
    constexpr auto isSome() const noexcept -> bool;

    constexpr auto isNone() const noexcept -> bool;

    constexpr explicit operator bool() const noexcept;

    constexpr auto operator*() -> T&;

    constexpr auto operator*() const -> const T&;

    constexpr auto value() -> T&;

    constexpr auto value() const -> const T&;

    constexpr auto operator->() noexcept -> T*;

    constexpr auto operator->() const noexcept -> const T*;

    // Comparison
    friend constexpr auto operator==(const Maybe& lhs, const Maybe& rhs) -> bool
    {
        return lhs.m_ptr == rhs.m_ptr;
    }

    template<typename U>
    friend constexpr auto operator==(const Maybe& lhs, const U& rhs) -> bool
    {
        return static_cast<bool>(lhs)
               ? *lhs.m_ptr == rhs
               : false;
    }

    friend constexpr auto operator==(const Maybe& lhs, const None&) noexcept -> bool
    {
        return lhs.m_ptr == nullptr;
    }

protected:
    T* m_ptr = nullptr;
};

template<typename T>
constexpr Maybe<T&>::Maybe(None) noexcept
{}

template<typename T>
constexpr Maybe<T&>::Maybe(const Maybe& other) noexcept
    : m_ptr(other.m_ptr)
{}

template<typename T>
constexpr Maybe<T&>::Maybe(Maybe&& other) noexcept
    : m_ptr(std::exchange(other.m_ptr, nullptr))
{}

template<typename T>
template<typename U>
requires (!IsMaybe<std::remove_cvref_t<U>>::value)
constexpr Maybe<T&>::Maybe(U&& ref) noexcept
    : m_ptr(std::addressof(ref))
{
    static_assert(std::is_lvalue_reference_v<U>, "ref must be an lvalue reference");
}

template<typename T>
constexpr auto Maybe<T&>::operator=(None) noexcept -> Maybe&
{
    m_ptr = nullptr;
    return *this;
}

template<typename T>
constexpr auto Maybe<T&>::operator=(const Maybe& other) noexcept -> Maybe&
{
    if (this != &other)
    {
        m_ptr = other.m_ptr;
    }

    return *this;
}

template<typename T>
constexpr auto Maybe<T&>::operator=(Maybe&& other) noexcept -> Maybe&
{
    if (this != &other)
    {
        m_ptr = std::exchange(other.m_ptr, nullptr);
    }

    return *this;
}

template<typename T>
template<typename U>
requires (!IsMaybe<std::remove_cvref_t<U>>::value)
constexpr auto Maybe<T&>::operator=(U&& value) -> Maybe&
{
    static_assert(std::is_lvalue_reference_v<decltype(value)>);
    m_ptr = std::addressof(value);
    return *this;
}

template<typename T>
constexpr auto Maybe<T&>::isSome() const noexcept -> bool
{
    return m_ptr != nullptr;
}

template<typename T>
constexpr auto Maybe<T&>::isNone() const noexcept -> bool
{
    return m_ptr == nullptr;
}

template<typename T>
constexpr Maybe<T&>::operator bool() const noexcept
{
    return m_ptr != nullptr;
}

template<typename T>
constexpr auto Maybe<T&>::operator*() -> T&
{
    if (isNone())
    {
        throw InvalidOperationException("Value is not present");
    }

    return *m_ptr;
}

template<typename T>
constexpr auto Maybe<T&>::operator*() const -> const T&
{
    if (isNone())
    {
        throw InvalidOperationException("Value is not present");
    }

    return *m_ptr;
}

template<typename T>
constexpr auto Maybe<T&>::value() -> T&
{
    if (isNone())
    {
        throw InvalidOperationException("Value is not present");
    }

    return *m_ptr;
}

template<typename T>
constexpr auto Maybe<T&>::value() const -> const T&
{
    if (isNone())
    {
        throw InvalidOperationException("Value is not present");
    }

    return *m_ptr;
}

template<typename T>
constexpr auto Maybe<T&>::operator->() noexcept -> T*
{
    return m_ptr;
}

template<typename T>
constexpr auto Maybe<T&>::operator->() const noexcept -> const T*
{
    return m_ptr;
}

} // namespace isml

#endif // ISML_MAYBE_HPP
