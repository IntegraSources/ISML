/**
 * @file    value_field.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    28/01/2020
 */

#ifndef ISML_VALUE_FIELD_HPP
#define ISML_VALUE_FIELD_HPP

#include <string_view>

#include <isml/serialization/serializers/binary_serializer.hpp>
#include <isml/message/field/field.hpp>

namespace isml {

template<typename T, typename Serializer = void>
class ValueField;

template<typename T, typename Serializer>
auto operator==(const ValueField<T, Serializer>& lhs, const ValueField<T, Serializer>& rhs) -> bool;

template<typename T, typename Serializer>
auto operator==(const ValueField<T, Serializer>& lhs, const T& rhs) -> bool;

/**
 * @class   DataField
 * @brief   Stores data and provides interface to access them with ability
 *          to modification.
 * @tparam  T Stored data type.
 * @since   0.1.0
 */

template<typename T, typename Serializer>
class ValueField : public Field
{
public:
    using Field::Field;
    explicit ValueField(const ValueField& other);

public:

    /**
     * Returns data.
     * @return Reference to storing data.
     */

    auto ref() noexcept -> T&;

    /**
     * Returns data.
     * @return Constant reference to storing data.
     */

    auto cref() const noexcept -> const T&;

    /**
     * Returns data.
     * @return Storing data by value.
     */

    auto get() const noexcept -> T;

    /**
     * Sets new value.
     * @param other New value.
     */

    auto set(const T& other) noexcept -> void;

    auto serialize(SerializationContext& context) const -> void override;
    auto deserialize(SerializationContext& context) -> void override;
    auto serializedSize() const noexcept -> std::size_t override;

    template<typename U = T>
    auto operator=(U&& value) -> ValueField&;

    auto clone() const noexcept -> Field::Ptr override;

    /// @copydoc Field::type()
    auto valueType() const noexcept -> const std::type_info& override;

    // Comparison
    friend auto operator==<T, Serializer>(const ValueField<T, Serializer>& lhs, const ValueField<T, Serializer>& rhs) -> bool;
    friend auto operator==<T, Serializer>(const ValueField<T, Serializer>& lhs, const T& rhs) -> bool;

protected:
    T m_value{};
};

template<typename T, typename Serializer>
inline ValueField<T, Serializer>::ValueField(const ValueField& other)
    : Field(other)
    , m_value(other.m_value)
{}

template<typename T, typename Serializer>
inline auto ValueField<T, Serializer>::ref() noexcept -> T&
{
    return m_value;
}

template<typename T, typename Serializer>
inline auto ValueField<T, Serializer>::cref() const noexcept -> const T&
{
    return m_value;
}

template<typename T, typename Serializer>
inline auto ValueField<T, Serializer>::get() const noexcept -> T
{
    return m_value;
}

template<typename T, typename Serializer>
inline auto ValueField<T, Serializer>::set(const T& other) noexcept -> void
{
    m_value = other;
}

template<typename T, typename Serializer>
inline auto ValueField<T, Serializer>::serialize(SerializationContext& context) const -> void
{
    Serializer::serialize(context, m_value, m_name);
}

template<typename T, typename Serializer>
inline auto ValueField<T, Serializer>::deserialize(SerializationContext& context) -> void
{
    Serializer::deserialize(context, m_value, m_name);
}

template<typename T, typename Serializer>
auto ValueField<T, Serializer>::serializedSize() const noexcept -> std::size_t
{
    return binary::size(m_value);
}

template<typename T, typename Serializer>
template<typename U>
auto ValueField<T, Serializer>::operator=(U&& value) -> ValueField&
{
    m_value = std::forward<U>(value);
    return *this;
}

template<typename T, typename Serializer>
inline auto ValueField<T, Serializer>::clone() const noexcept -> Field::Ptr
{
    return std::unique_ptr<ValueField>(new ValueField(*this));
}

template<typename T, typename Serializer>
auto ValueField<T, Serializer>::valueType() const noexcept -> const std::type_info&
{
    return typeid(T);
}

template<typename T, typename Serializer>
auto operator==(const ValueField<T, Serializer>& lhs, const ValueField<T, Serializer>& rhs) -> bool
{
    return lhs.name() == rhs.name()
        && lhs.get() == rhs.get();
}

template<typename T, typename Serializer>
auto operator==(const ValueField<T, Serializer>& lhs, const T& rhs) -> bool
{
    return lhs.ref() == rhs;
}

} // namespace isml

#endif // ISML_VALUE_FIELD_HPP
