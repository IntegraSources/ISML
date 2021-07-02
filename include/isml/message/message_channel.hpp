/**
 * @file    message_channel.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    5/19/20
 */

#ifndef ISML_MESSAGE_CHANNEL_HPP
#define ISML_MESSAGE_CHANNEL_HPP

#include <isml/message/message.hpp>

namespace isml {

/**
 * @class   MessageChannel
 * @brief   Serves as a base class for all message channel types.
 */

class MessageChannel
{
public:
    virtual ~MessageChannel() = default;

    MessageChannel(const MessageChannel&) = delete;
    auto operator=(const MessageChannel&) -> MessageChannel& = delete;

    MessageChannel(MessageChannel&&) = default;
    auto operator=(MessageChannel&&) -> MessageChannel& = default;

protected:
    MessageChannel() = default;

public:
    auto send(Message::Ptr msg) -> void;

protected:
    virtual auto internal_send(Message::Ptr msg) -> void = 0;
};

} // namespace isml

#endif // ISML_MESSAGE_CHANNEL_HPP
