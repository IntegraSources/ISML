/**
 * @file    exceptions.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    22.01.2020
 */

#ifndef ISML_MESSAGE_EXCEPTIONS_HPP
#define ISML_MESSAGE_EXCEPTIONS_HPP

#include <isml/base_types.hpp>
#include <isml/exceptions.hpp>

namespace isml {

/**
 * @class   UnknownMessageTypeException
 * @brief   Indicates that an unknown type of message was encountered.
 */

class UnknownMessageTypeException : public MessageException
{
public:
    UnknownMessageTypeException(std::string message, MessageType messageType) noexcept;

public:

    /**
     * @brief   Returns the unknown message type.
     * @return  Message type.
     */

    auto messageType() const noexcept -> MessageType;

private:
    const MessageType m_messageType; ///< Message type.
};

/**
 * @class   FieldNotFoundException
 * @brief   Indicates that field with given name does not exist.
 */

class FieldDoesNotExistException : public MessageException
{
public:

    /**
     * @brief   Constructor
     * @param   message     Error description.
     * @param   field_name  Field name.
     * @param   message_id  Message ID.
     */

    FieldDoesNotExistException(std::string message, std::string field_name, SessionId session_id, MessageId message_id) noexcept;

public:

    /**
     * @brief   Returns name of non-existent field.
     */

    auto fieldName() const noexcept -> const std::string&;

    /**
     * @brief   Returns source session ID.
     */

    auto sessionId() const noexcept -> SessionId;


    /**
     * @brief   Return source message ID.
     */

    auto messageId() const noexcept -> MessageId;

private:
    std::string m_field_name; ///< Field name.
    SessionId   m_session_id; ///< Session ID.
    MessageId   m_message_id; ///< Message ID.
};

/**
 * @class   DuplicateFieldException
 * @brief   Indicates that field with given name could have been duplicated.
 */

class DuplicateFieldException : public MessageException
{
public:
    DuplicateFieldException(std::string message, std::string field_name) noexcept;

public:

    /**
     * @brief   Returns name of duplicated field.
     */

    auto fieldName() const noexcept -> const std::string&;

private:
    const std::string m_field_name; ///< Field name.
};

ISML_DEFINE_EXCEPTION(MessageException, InconsistentProtocolVersionException)

} // namespace isml

#endif // ISML_MESSAGE_EXCEPTIONS_HPP
