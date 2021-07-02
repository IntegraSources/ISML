/**
 * @file    message_descriptor.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    29/01/2020
 */

#include <isml/message/message_descriptor.hpp>
#include <isml/exceptions.hpp>
#include <isml/io/format.hpp>

namespace isml {

MessageDescriptor::MessageDescriptor(const MessageDescriptor& other)
    : m_type(other.m_type)
{
    copy(other);
}

MessageDescriptor::MessageDescriptor(MessageDescriptor&& other) noexcept
    : m_type(other.m_type)
{
    swap(other);
}

MessageDescriptor::MessageDescriptor(MessageType type) noexcept
    : m_type(type)
{}

auto MessageDescriptor::operator=(const MessageDescriptor& other) -> MessageDescriptor&
{
    if (this != &other)
    {
        copy(other);
    }

    return *this;
}

auto MessageDescriptor::operator=(MessageDescriptor&& other) noexcept -> MessageDescriptor&
{
    if (this != &other)
    {
        swap(other);
    }

    return *this;
}

auto MessageDescriptor::type() const noexcept -> MessageType
{
    return m_type;
}

auto MessageDescriptor::registerField(FieldDescriptor::Ptr descriptor) -> MessageDescriptor&
{
    addFieldDescriptor(std::move(descriptor));
    return *this;
}

auto MessageDescriptor::addFieldDescriptor(FieldDescriptor::Ptr descriptor) -> void
{
    if (m_field_descriptors_by_name.contains(descriptor->name()))
        throw InvalidArgumentException(fmt::format("Descriptor with same name ({}) has already been registered", descriptor->name()));

    const auto& name = descriptor->name();
    auto it = m_field_descriptors.empty() ? m_field_descriptors.begin() : std::prev(m_field_descriptors.end());
    it = m_field_descriptors.insert(it, std::move(descriptor));
    m_field_descriptors_by_name.insert(std::make_pair(name, it));
}

auto MessageDescriptor::fieldDescriptors() const noexcept -> const FieldDescriptors&
{
    return m_field_descriptors;
}

auto MessageDescriptor::swap(MessageDescriptor& other) noexcept -> void
{
    std::swap(m_type, other.m_type);
    std::swap(m_field_descriptors, other.m_field_descriptors);
    std::swap(m_field_descriptors_by_name, other.m_field_descriptors_by_name);
}

auto MessageDescriptor::copy(const MessageDescriptor& other) -> void
{
    m_type = other.m_type;

    m_field_descriptors.clear();
    m_field_descriptors_by_name.clear();

    auto it = m_field_descriptors.begin();
    for (const auto& descriptor : other.m_field_descriptors)
    {
        it = m_field_descriptors.insert(it, descriptor->clone());
        m_field_descriptors_by_name.insert(std::make_pair(descriptor->name(), it));
    }
}

} // namespace isml
