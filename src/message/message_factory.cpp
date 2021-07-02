/**
 * @file    message_factory.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    22.01.2020
 */

#include <isml/message/message_factory.hpp>

#include <memory>
#include <utility>

#include <isml/message/exceptions.hpp>

#include <isml/session/session.hpp>

namespace isml {

MessageFactory::MessageFactory(const MessageFactory& other)
    : m_descriptors(other.m_descriptors)
{}

MessageFactory::MessageFactory(MessageFactory&& other) noexcept
{
    swap(other);
}

auto MessageFactory::operator=(const MessageFactory& other) -> MessageFactory&
{
    if (this != &other)
    {
        m_descriptors = other.m_descriptors;
    }

    return *this;
}

auto MessageFactory::operator=(MessageFactory&& other) noexcept -> MessageFactory&
{
    if (this != &other)
    {
        swap(other);
    }

    return *this;
}

auto MessageFactory::getInstance() -> MessageFactory&
{
    static MessageFactory instance;
    return instance;
}

auto MessageFactory::createMessage(MessageType type, Session& session) -> Message::Ptr
{
    if (!m_descriptors.contains(type))
        throw UnknownMessageTypeException("There is no registered descriptor for this message type", type);

    const auto& descriptor = m_descriptors.at(type);
    return std::make_unique<Message>(descriptor, session.shared_from_this());
}

auto MessageFactory::addDescriptor(MessageDescriptor descriptor) noexcept -> bool
{
    const auto type = descriptor.type();
    const auto [_, inserted] =
        m_descriptors.insert(std::make_pair(type, std::move(descriptor)));
    return inserted;
}

auto MessageFactory::hasDescriptor(MessageType type) const noexcept -> bool
{
    return m_descriptors.contains(type);
}

auto MessageFactory::swap(MessageFactory& other) noexcept -> void
{
    std::swap(m_descriptors, other.m_descriptors);
}

} // namespace isml
