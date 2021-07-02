/**
 * @file    field.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    28/01/2020
 */

#include <isml/message/field/field.hpp>

#include <utility>

namespace isml {

Field::Field(const Field& other)
    : m_name(other.m_name)
{}

Field::Field(std::string name) noexcept
    : m_name(std::move(name))
{}

auto Field::name() const noexcept -> const std::string&
{
    return m_name;
}

} // namespace isml
