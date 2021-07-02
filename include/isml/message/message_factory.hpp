/**
 * @file    message_factory.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    21.01.2020
 */

#ifndef ISML_MESSAGE_FACTORY_HPP
#define ISML_MESSAGE_FACTORY_HPP

#include <unordered_map>

#include <isml/base_types.hpp>

#include <isml/message/message.hpp>
#include <isml/message/message_descriptor.hpp>

namespace isml {

class Session;

/**
 * @class  MessageFactory
 * @brief  A factory for creating messages.
 */

class MessageFactory final
{
protected:
    using Descriptors = std::unordered_map<MessageType, MessageDescriptor>;

public:
    MessageFactory() = default;
    MessageFactory(const MessageFactory& other);
    MessageFactory(MessageFactory&& other) noexcept;

    ~MessageFactory() = default;

    auto operator=(const MessageFactory& other) -> MessageFactory&;
    auto operator=(MessageFactory&& other) noexcept-> MessageFactory&;

public:
    /// Get factory instance.
    static auto getInstance() -> MessageFactory&;

    /**
     * @brief   Creates message instance by message type.
     *
     * @param   type  Message type.
     *
     * @return  If successful - pointer to message, otherwise - exception
     *          will be thrown.
     *
     * @throw   UnknownMessageTypeException - message type is not registered,
     *          cannot create message of specified type.
     *
     * @code
     * auto msg = factory.createMessage(MessageType::EnableTraceLog);
     * @endcode
     */

    auto createMessage(MessageType type, Session& session) -> Message::Ptr;

    /**
     * @brief   Adds a new message descriptor.
     *
     * @param   descriptor  Message descriptor.
     *
     * @return  If successful - true, otherwise - false.
     */

    auto addDescriptor(MessageDescriptor descriptor) noexcept -> bool;

    template<typename ConfigureDescriptor>
    auto addDescriptor(MessageType type, ConfigureDescriptor&& configure) -> bool;

    auto hasDescriptor(MessageType type) const noexcept -> bool;

    auto swap(MessageFactory& other) noexcept -> void;

protected:
    Descriptors m_descriptors {}; ///< Message descriptors.
};

template<typename ConfigureDescriptor>
auto MessageFactory::addDescriptor(MessageType type, ConfigureDescriptor&& configure) -> bool
{
    MessageDescriptor descriptor { type };
    configure(descriptor);
    return addDescriptor(std::move(descriptor));
}

} // namespace isml

#endif // ISML_MESSAGE_FACTORY_HPP
