/**
 * @file    message_channel.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    5/19/20
 */

#include <isml/message/message_channel.hpp>

namespace isml {

auto MessageChannel::send(Message::Ptr msg) -> void
{
    internal_send(std::move(msg));
}

} // namespace isml
