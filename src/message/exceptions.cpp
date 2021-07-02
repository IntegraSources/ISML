/**
 * @file    exceptions.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    22.01.2020
 */

#include <isml/message/exceptions.hpp>

#include <utility>

namespace isml {

UnknownMessageTypeException::UnknownMessageTypeException(std::string message, MessageType messageType) noexcept
    : MessageException(std::move(message))
    , m_messageType(messageType)
{}

auto UnknownMessageTypeException::messageType() const noexcept -> MessageType
{
    return m_messageType;
}

FieldDoesNotExistException::FieldDoesNotExistException(std::string message, std::string field_name, SessionId session_id, MessageId message_id) noexcept
    : MessageException(std::move(message))
    , m_field_name(std::move(field_name))
    , m_session_id(session_id)
    , m_message_id(message_id)
{}

auto FieldDoesNotExistException::fieldName() const noexcept -> const std::string&
{
    return m_field_name;
}

auto FieldDoesNotExistException::sessionId() const noexcept -> SessionId
{
    return m_session_id;
}

auto FieldDoesNotExistException::messageId() const noexcept -> MessageId
{
    return m_message_id;
}

DuplicateFieldException::DuplicateFieldException(std::string message, std::string field_name) noexcept
    : MessageException(std::move(message))
    , m_field_name(std::move(field_name))
{}

auto DuplicateFieldException::fieldName() const noexcept -> const std::string&
{
    return m_field_name;
}

} // namespace isml
