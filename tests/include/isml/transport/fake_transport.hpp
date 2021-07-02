/**
 * @file    fake_transport.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    19.04.2020
 */

#ifndef ISML_FAKE_TRANSPORT_HPP
#define ISML_FAKE_TRANSPORT_HPP

#include <isml/transport/transport.hpp>

namespace isml {

class FakeTransport : public Transport
{
public:
    FakeTransport() = default;

public:
    auto removeExpiredRequests() -> void override {}

private:
    void doStart() override { m_state = State::Started; }
    void doStop() override { m_state = State::Stopped; }
    void doSend(Message::Ptr) override {}
    auto doInit() -> void override {}
    auto doReceive() -> std::optional<Message::Ptr> override { return std::nullopt; }
    auto doRequest(Message::Ptr) -> FutureMessage override { return {}; }
};

} // namespace isml

#endif // ISML_FAKE_TRANSPORT_HPP
