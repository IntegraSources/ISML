/**
 * @file    result.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    10/22/20
 */

#ifndef ISML_RESULT_HPP
#define ISML_RESULT_HPP

#include <variant>
#include <type_traits>

#include <isml/base/base_types.hpp>
#include <isml/utility/value_holder.hpp>

namespace isml {

template<typename T, typename E>
class Result;

template<typename T>
class Success;

template<typename E>
class Failure;

template<typename T, typename E>
class Result
{
public:
    using Value = T;
    using Error = E;

    static_assert(!std::is_same_v<T, E>);

public:
    Result() = default;
    explicit Result(Value value);

    Result(const Success<T>& success);
    Result(Success<T>&& success);

    Result(const Failure<E>& failure);
    Result(Failure<E>&& failure);

    explicit operator bool() const noexcept;

    auto success() const noexcept -> bool;
    auto failure() const noexcept -> bool;

    auto value() -> Value&;
    auto value() const -> const Value&;

    auto error() -> Error&;
    auto error() const -> const Error&;

    auto operator->() -> std::add_pointer_t<Value>;
    auto operator->() const -> std::add_pointer_t<const Value>;

protected:
    std::variant<std::monostate, Value, Error> m_variant {};
};

template<typename T>
class Success
    : public ValueHolder<T>
{
public:
    using ValueHolder<T>::ValueHolder;
};

template<typename E>
class Failure
    : public ValueHolder<E>
{
public:
    using ValueHolder<E>::ValueHolder;
};

template<typename T, typename E>
inline Result<T, E>::Result(Value value)
    : m_variant(std::move(value))
{}

template<typename T>
Result(T) -> Result<T, None>;

template<typename T, typename E>
inline Result<T, E>::Result(const Success<T>& success)
    : m_variant(success.get())
{}

template<typename T, typename E>
inline Result<T, E>::Result(Success<T>&& success)
    : m_variant(std::move(success.get()))
{}

template<typename T, typename E>
inline Result<T, E>::Result(const Failure<E>& failure)
    : m_variant(failure.get())
{}

template<typename T, typename E>
inline Result<T, E>::Result(Failure<E>&& failure)
    : m_variant(std::move(failure.get()))
{}

template<typename T, typename E>
inline Result<T, E>::operator bool() const noexcept
{
    return std::holds_alternative<Value>(m_variant);
}

template<typename T, typename E>
inline auto Result<T, E>::success() const noexcept -> bool
{
    return std::holds_alternative<Value>(m_variant);
}

template<typename T, typename E>
inline auto Result<T, E>::failure() const noexcept -> bool
{
    return std::holds_alternative<Error>(m_variant);
}

template<typename T, typename E>
inline auto Result<T, E>::value() -> Value&
{
    return std::get<Value>(m_variant);
}

template<typename T, typename E>
inline auto Result<T, E>::value() const -> const Value&
{
    return std::get<Value>(m_variant);
}

template<typename T, typename E>
inline auto Result<T, E>::error() -> Error&
{
    return std::get<Error>(m_variant);
}

template<typename T, typename E>
inline auto Result<T, E>::error() const -> const Error&
{
    return std::get<Error>(m_variant);
}

template<typename T, typename E>
inline auto Result<T, E>::operator->() -> std::add_pointer_t<Value>
{ return &value(); }

template<typename T, typename E>
inline auto Result<T, E>::operator->() const -> std::add_pointer_t<const Value>
{ return &value(); }

template<typename T>
Success(T) -> Success<T>;

template<typename E>
Failure(E) -> Failure<E>;

} // namespace isml

#endif // ISML_RESULT_HPP
