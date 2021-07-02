/**
 * @file    transport.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    13/02/2020
 */

#include <isml/transport/transport.hpp>

#include <utility>

#include <isml/exceptions.hpp>

namespace isml {

auto Transport::send(Message::Ptr msg) -> void
{
    doSend(std::move(msg));
}

auto Transport::receive() -> std::optional<Message::Ptr>
{
    return doReceive();
}

auto Transport::request(Message::Ptr msg) -> FutureMessage
{
    return doRequest(std::move(msg));
}

auto Transport::setOwner(Session& session) -> void
{
    m_session = &session;
}

auto Transport::owner() -> Session&
{
    if (!m_session)
    {
        throw NullPointerException("Attempt to dereference a null pointer [throwable=Session::owner]");
    }

    return *m_session;
}

} // namespace isml
