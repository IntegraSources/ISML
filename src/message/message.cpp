/**
 * @file    message.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    21.01.2020
 */

#include <isml/message/message.hpp>

#include <utility>
#include <atomic>

#include <isml/message/message_descriptor.hpp>
#include <isml/session/session.hpp>

namespace isml {

static auto generateId() -> MessageId
{
    static std::atomic<MessageId> id { 0 };
    return ++id;
}

Message::Message(const MessageDescriptor& descriptor, Session::Ptr session)
    : m_id(generateId())
    , m_type(descriptor.type())
    , m_fieldset()
    , m_session(std::move(session))
{
    for (const auto& field_descriptor : descriptor.fieldDescriptors())
        m_fieldset.add(field_descriptor->createField());
}

Message::Message(const Message& other)
    : m_id(generateId())
    , m_type(other.m_type)
    , m_fieldset(other.m_fieldset)
    , m_session(other.m_session)
{}

Message::Message(Message&& other) noexcept
    : m_id(other.m_id)
    , m_type(other.m_type)
    , m_fieldset(std::move(other.m_fieldset))
    , m_session(std::move(other.m_session))
{}

auto Message::type() const noexcept -> MessageType
{
    return m_type;
}

auto Message::session() noexcept -> Session::Ptr&
{
    return m_session;
}

auto Message::id() const noexcept -> MessageId
{
    return m_id;
}

auto Message::hasField(const std::string& name) const noexcept -> bool
{
    return m_fieldset.contains(name);
}

auto Message::clone() const noexcept -> Message::Ptr
{
    return std::make_unique<Message>(*this);
}

auto Message::serialize(SerializationContext& context) const -> void
{
    return m_fieldset.serialize(context);
}

auto Message::deserialize(SerializationContext& context) -> void
{
    return m_fieldset.deserialize(context);
}

auto Message::serializedSize() const noexcept -> std::size_t
{
    return m_fieldset.serializedSize();
}

auto Message::sessionId() const noexcept -> SessionId
{
    return m_session ? m_session->id() : kBadSessionId;
}

} // namespace isml
