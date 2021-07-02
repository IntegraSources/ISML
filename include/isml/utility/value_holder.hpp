/**
 * @file    value_holder.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    10/22/20
 */

#ifndef ISML_VALUE_HOLDER_HPP
#define ISML_VALUE_HOLDER_HPP

#include <utility>

namespace isml {

template<typename T = void>
class ValueHolder;

template<>
class ValueHolder<void>
{
public:
    virtual ~ValueHolder() = default;
};

template<typename T>
class ValueHolder
    : public ValueHolder<>
{
public:
    using Value = T;

public:
    ValueHolder() = delete;
    explicit ValueHolder(const Value& value);
    explicit ValueHolder(Value&& value);

public:
    auto get() noexcept -> Value&;
    auto get() const noexcept -> const Value&;

protected:
    Value m_value;
};

template<typename T>
inline ValueHolder<T>::ValueHolder(const Value& value)
    : m_value(value)
{}

template<typename T>
inline ValueHolder<T>::ValueHolder(Value&& value)
    : m_value(std::move(value))
{}

template<typename T>
inline auto ValueHolder<T>::get() const noexcept -> const Value&
{
    return m_value;
}

template<typename T>
inline auto ValueHolder<T>::get() noexcept -> Value&
{
    return m_value;
}

template<typename T>
ValueHolder(T) -> ValueHolder<T>;

} // namespace isml

#endif // ISML_VALUE_HOLDER_HPP
