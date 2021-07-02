/**
 * @file    message.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    21.01.2020
 */

#ifndef ISML_MESSAGE_HPP
#define ISML_MESSAGE_HPP

#include <vector>
#include <future>
#include <memory>

#include <isml/base_types.hpp>

#include <isml/message/exceptions.hpp>
#include <isml/message/field/field.hpp>
#include <isml/message/field/field_set.hpp>
#include <isml/message/field/value_field.hpp>

#include <isml/serialization/serializable.hpp>

namespace isml {

class Session;
class MessageDescriptor;

/**
 * @class   Message
 * @brief   Communication unit in messaging system.
 */

class Message : public Serializable
{
public:
    using Ptr = std::unique_ptr<Message>;
    using Fields = std::vector<std::pair<std::string, Field::Ptr>>;

public:
    explicit Message(const MessageDescriptor& descriptor, std::shared_ptr<Session> session);
    Message(const Message& other);
    Message(Message&& other) noexcept;

    virtual ~Message() = default;

    auto operator=(Message&&) -> Message& = delete;
    auto operator=(const Message&) -> Message& = delete;

public:
    auto type() const noexcept -> MessageType;

    /**
     * Returns field converted to the data field of specified type.
     * @tparam  T       Target data field type.
     * @param   name    Field name.
     * @return  If success - returns data field, otherwise - exception will
     *          be thrown.
     *
     * @throw   FieldDoesNotExistException - will be thrown if field with such
     *          name does not exist.
     * @throw   InvalidCastException - will be thrown if target field type does
     *          not match the real type of field with such name.
     *          Note: depends on build type.
     */

    template<typename T>
    auto field(const std::string& name) -> ValueField<T>&;

    /**
     * Returns the associated session.
     */

    auto session() noexcept -> std::shared_ptr<Session>&;

    /**
     * Returns message identifier.
     */

    auto id() const noexcept -> MessageId;

    /**
     * Checks if field exists.
     * @param  name  Field name.
     *
     * @return  If field exists - true,
     *          otherwise - false.
     */

    auto hasField(const std::string& name) const noexcept -> bool;

    auto clone() const noexcept -> Message::Ptr;

    auto serialize(SerializationContext& context) const -> void override;
    auto deserialize(SerializationContext& context) -> void override;

    auto serializedSize() const noexcept -> std::size_t;

protected:
    auto sessionId() const noexcept -> SessionId;

protected:
    MessageId                m_id;
    MessageType              m_type;
    FieldSet                 m_fieldset;
    std::shared_ptr<Session> m_session;
};

template<typename T>
auto Message::field(const std::string& name) -> ValueField<T>&
{
    if (auto maybe_field = m_fieldset.get<T>(name))
        return maybe_field.value();

    throw FieldDoesNotExistException("Message doesn't contain specified field", name, sessionId(), m_id);
}

using FutureMessage = std::future<Message::Ptr>;

} // namespace isml

#endif // ISML_MESSAGE_HPP
