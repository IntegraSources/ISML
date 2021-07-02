/**
 * @file    property.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    7/23/20
 */

#ifndef ISML_UTILITY_PROPERTY_HPP
#define ISML_UTILITY_PROPERTY_HPP

#include <any>
#include <optional>

namespace isml {

/**
 * @class   Property
 * @brief   A container for value of any type.
 */

class Property
{
public:
    Property() = default;
    virtual ~Property() = default;

    template<typename T>
        requires (!std::is_same_v<T, Property>)
    explicit Property(T&& value);

    template<typename T>
        requires (!std::is_same_v<T, Property>)
    auto operator=(T&& value) -> Property&;

public:
    template<typename T>
    auto as() const -> T;

    template<typename T>
    auto safe_as() const noexcept -> std::optional<T>;

    template<typename T>
    auto safe_as(const T& default_value) -> T;

    auto valueless() const noexcept -> bool;

    template<typename T>
    friend auto operator==(const Property& lhs, const T& rhs) -> bool;

protected:
    std::any m_value {};
};

template<typename T>
    requires (!std::is_same_v<T, Property>)
inline Property::Property(T&& value)
    : m_value(std::forward<value>())
{}

ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic push)
ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Weffc++")

template<typename T>
    requires (!std::is_same_v<T, Property>)
inline auto Property::operator=(T&& value) -> Property&
{
    m_value = std::forward<T>(value);
    return *this;
}

ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic pop)

template<typename T>
inline auto Property::as() const -> T
{
    return std::any_cast<T>(m_value);
}

template<typename T>
auto Property::safe_as() const noexcept -> std::optional<T>
{
    return m_value.type() == typeid(T)
         ? std::make_optional(std::any_cast<T>(m_value))
         : std::nullopt;
}

template<typename T>
auto Property::safe_as(const T& default_value) -> T
{
    return m_value.type() == typeid(T)
         ? std::any_cast<T>(m_value)
         : default_value;
}

inline auto Property::valueless() const noexcept -> bool
{
    return !m_value.has_value();
}

template<typename T>
inline auto operator==(const Property& lhs, const T& rhs) -> bool
{
    if (lhs.m_value.type() == typeid(T))
        return std::any_cast<T>(lhs.m_value) == rhs;
    return false;
}

} // namespace isml

#endif // ISML_UTILITY_PROPERTY_HPP
