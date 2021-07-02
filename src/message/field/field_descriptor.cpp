/**
 * @file    field_descriptor.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    28/01/2020
 */

#include <isml/message/field/field_descriptor.hpp>

#include <utility>

namespace isml {

FieldDescriptor::FieldDescriptor(const FieldDescriptor& other)
    : m_name(other.m_name)
{}

FieldDescriptor::FieldDescriptor(FieldDescriptor&& other) noexcept
    : m_name(std::move(other.m_name))
{}

FieldDescriptor::FieldDescriptor(std::string name)
    : m_name(std::move(name))
{}

auto FieldDescriptor::name() const noexcept -> const std::string&
{
    return m_name;
}

} // namespace isml
