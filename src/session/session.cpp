/**
 * @file    session.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    10/02/2020
 */

#include <isml/session/session.hpp>

#include <utility> // for std::exchange

namespace isml {

Session::Session(SessionId id, std::unique_ptr<Transport> transport)
    : m_id(id)
    , m_transport(std::move(transport))
{
    m_transport->setOwner(*this);
    m_transport->start();
}

auto Session::id() const noexcept -> SessionId
{
    return m_id;
}

auto Session::send(Message::Ptr msg) -> void
{
    m_transport->send(std::move(msg));
}

auto Session::receive() -> std::optional<Message::Ptr>
{
    return m_transport->receive();
}

auto Session::request(Message::Ptr msg) -> FutureMessage
{
    return m_transport->request(std::move(msg));
}

auto Session::shutdown() -> void
{
    m_transport->stop();
}

auto Session::active() const noexcept -> bool
{
    return m_transport->state() == Service::State::StartPending
        or m_transport->state() == Service::State::Started;
}

auto Session::transport() noexcept -> std::unique_ptr<Transport>&
{
    return m_transport;
}

auto Session::createNew(SessionId session_id, std::unique_ptr<Transport> transport) -> Session::Ptr
{
    return std::make_shared<Session>(session_id, std::move(transport));
}

} // namespace isml
