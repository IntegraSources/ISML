/**
 * @file    message_descriptor.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    28/01/2020
 */

#ifndef ISML_MESSAGE_DESCRIPTOR_HPP
#define ISML_MESSAGE_DESCRIPTOR_HPP

#include <list>
#include <unordered_map>

#include <isml/message/exceptions.hpp>
#include <isml/message/field/field_descriptor.hpp>
#include <isml/message/field/value_field_descriptor.hpp>

namespace isml {

/**
 * @class   MessageDescriptor
 * @brief   Contains descriptors for creating all necessary fields in a message
 *          of specified type.
 */

class MessageDescriptor
{
public:
    using FieldDescriptors = std::list<FieldDescriptor::Ptr>;
    using FieldDescriptorsByName = std::unordered_map<std::string, FieldDescriptors::iterator>;

public:
    MessageDescriptor() = delete;
    MessageDescriptor(const MessageDescriptor& other);
    MessageDescriptor(MessageDescriptor&& other) noexcept;
    explicit MessageDescriptor(MessageType type) noexcept;

    ~MessageDescriptor() = default;

    auto operator=(const MessageDescriptor& other) -> MessageDescriptor&;
    auto operator=(MessageDescriptor&& other) noexcept -> MessageDescriptor&;

public:
    /// Gets message type.
    auto type() const noexcept -> MessageType;

    template<typename Serializer, typename T>
    auto registerField(std::string name) -> MessageDescriptor&;

    /**
     * @brief   Registers a new field.
     *
     * @param   descriptor  Field descriptor.
     *
     * @return  Reference to itself.
     */

    auto registerField(FieldDescriptor::Ptr descriptor) -> MessageDescriptor&;

    /// Gets a field descriptor list.
    auto fieldDescriptors() const noexcept -> const FieldDescriptors&;

    /// Swap.
    auto swap(MessageDescriptor& other) noexcept -> void;

protected:
    /// Copies other descriptor.
    auto copy(const MessageDescriptor& other) -> void;

    auto addFieldDescriptor(FieldDescriptor::Ptr descriptor) -> void;

protected:
    MessageType             m_type;
    FieldDescriptors        m_field_descriptors         {};
    FieldDescriptorsByName  m_field_descriptors_by_name {};
};

template<typename Serializer, typename T>
inline auto MessageDescriptor::registerField(std::string name) -> MessageDescriptor&
{
    auto field = std::make_unique<ValueFieldDescriptor<T, Serializer>>(std::move(name));
    addFieldDescriptor(std::move(field));
    return *this;
}

} // namespace isml

#endif // ISML_MESSAGE_DESCRIPTOR_HPP
