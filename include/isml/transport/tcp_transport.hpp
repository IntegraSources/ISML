/**
 * @file    tcp_transport.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/02/2020
 */

#ifndef ISML_TCP_TRANSPORT_HPP
#define ISML_TCP_TRANSPORT_HPP

#include <memory>
#include <unordered_map>
#include <chrono>
#include <mutex>

ISML_DISABLE_WARNINGS_PUSH
#   include <boost/asio/ip/tcp.hpp>
ISML_DISABLE_WARNINGS_POP

#include <isml/base/maybe.hpp>

#include <isml/message/message_queue.hpp>

#include <isml/transport/transport.hpp>

namespace isml {

using TcpSocket = boost::asio::ip::tcp::socket;

/**
 * @class   TcpTransport
 * @brief   A message transport working over a TCP connection.
 */

class TcpTransport : public Transport
{
public:
    using Clock = std::chrono::high_resolution_clock;
    using Timestamp = Clock::time_point;
    using ByteBuffer = std::unique_ptr<char[]>;
    using Request = std::promise<Message::Ptr>;
    using PendingRequests = std::unordered_map<MessageId, Request>;
    using PendingRequestsTs = std::unordered_map<MessageId, Timestamp>;

public:
    TcpTransport() = delete;
    explicit TcpTransport(TcpSocket socket);
    TcpTransport(const TcpTransport&) = delete;

    auto operator=(const TcpTransport&) -> TcpTransport& = delete;

public:
    auto removeExpiredRequests() -> void override;

protected:
    auto writeMessage() -> void;

    auto readMessageLength() ->void;
    auto readMessage() -> void;
    auto onMessageRead() -> void;
    auto createMessageFromStream(std::stringstream& stream) -> Maybe<Message::Ptr>;

    auto resizeIncomingDataBuffer() -> void;
    auto resizeOutgoingDataBuffer() -> void;

    auto disconnected(const std::error_code& ec) -> bool;

private:
    // Interface: Service
    auto doStart() -> void override;
    auto doInit() -> void override;
    auto doStop() -> void override;

    // Interface: Transport
    auto doSend(Message::Ptr msg) -> void override;
    auto doReceive() -> std::optional<Message::Ptr> override;
    auto doRequest(Message::Ptr msg) -> FutureMessage override;

protected:
    TcpSocket               m_socket;
    PendingRequests         m_pending_requests      {};
    PendingRequestsTs       m_pending_requests_ts   {};
    std::mutex              m_pending_requests_mtx  {};

    ConcurrentMessageQueue  m_outgoing_messages     {};
    ByteBuffer              m_outgoing_data_buffer  {};
    MessageLength           m_outgoing_data_length  {};

    ConcurrentMessageQueue  m_incoming_messages     {};
    ByteBuffer              m_incoming_data_buffer  {};
    MessageLength           m_incoming_data_length  {};
};

} // namespace isml

#endif // ISML_TCP_TRANSPORT_HPP
