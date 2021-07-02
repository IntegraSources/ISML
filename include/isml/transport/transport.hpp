/**
 * @file    transport.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/02/2020
 */

#ifndef ISML_TRANSPORT_HPP
#define ISML_TRANSPORT_HPP

#include <memory>
#include <optional>

#include <isml/base/listenable.hpp>

#include <isml/message/message.hpp>
#include <isml/service/service.hpp>
#include <isml/transport/transport_listener.hpp>

namespace isml {

class Session;

/**
 * @class   Transport
 * @brief   Defines the interface for interaction with a message transport
 *          layer - the connection-side endcap to the message communication
 *          stack.
 */

class Transport
    : public Service
    , public Listenable<TransportListener>
{
public:
    using Ptr = std::unique_ptr<Transport>;

public:
    Transport() = default;
    Transport(const Transport&) = delete;
    Transport(Transport&&) = delete;

    auto operator=(const Transport&) -> Transport& = delete;
    auto operator=(Transport&&) -> Transport& = delete;

public:

    /**
     * @brief   Puts a message into an outgoing message queue.
     *
     * @param   msg  Message to be sent.
     */

    auto send(Message::Ptr msg) -> void;

    /**
     * @brief   Gets a received message.
     *
     * @return  If a message queue is not empty - message will be returned,
     *          otherwise - std::nullopt.
     */

    auto receive() -> std::optional<Message::Ptr>;

    /**
     * @brief   Puts message into an outgoing message queue and provide promise for
     *          awaiting the response.
     *
     * @param   msg  Message to be sent.
     *
     * @return  Promise.
     */

    auto request(Message::Ptr msg) -> FutureMessage;

    auto setOwner(Session& session) -> void;

    auto owner() -> Session&;

    /**
     * @brief   Removes expired requests from transport.
     */

    virtual auto removeExpiredRequests() -> void = 0;

private:
    virtual auto doSend(Message::Ptr msg) -> void = 0;
    virtual auto doReceive() -> std::optional<Message::Ptr> = 0;
    virtual auto doRequest(Message::Ptr msg) -> FutureMessage = 0;

protected:
    Session* m_session {};
};

} // namespace isml

#endif // ISML_TRANSPORT_HPP
