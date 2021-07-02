/**
 * @file    tcp_transport.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/02/2020
 */

#include <isml/transport/tcp_transport.hpp>

#include <sstream>
#include <cassert>
#include <algorithm>
#include <vector>

ISML_DISABLE_WARNINGS_PUSH
#  include <boost/asio/write.hpp>
#  include <boost/asio/read.hpp>
ISML_DISABLE_WARNINGS_POP

#include <isml/serialization/serializers/binary_serializer.hpp>
#include <isml/serialization/serialization_utility.hpp>

#include <isml/message/message_factory.hpp>

#include <isml/session/session.hpp>

using namespace std::chrono_literals;

namespace isml {

TcpTransport::TcpTransport(TcpSocket socket)
    : m_socket(std::move(socket))
{}

auto TcpTransport::doStart() -> void
{
    m_state = Service::State::Started;
    readMessageLength();
}

auto TcpTransport::doInit() -> void
{}

auto TcpTransport::doStop() -> void
{
    try
    {
        // I'm not sure that we really need to explicitly cancel asynchronous IO
        // operations before closing the socket.
        m_socket.cancel();
        m_socket.close();
    }
    catch (const std::exception& ex)
    {
        // Do nothing
    }
}

auto TcpTransport::doSend(Message::Ptr msg) -> void
{
    const auto in_progress = m_outgoing_messages.size() > 0;
    m_outgoing_messages.push(std::move(msg));
    if (!in_progress)
    {
        writeMessage();
    }
}

auto TcpTransport::doReceive() -> std::optional<Message::Ptr>
{
    return (m_incoming_messages.size() > 0)
         ? std::make_optional(m_incoming_messages.pull())
         : std::nullopt;
}

auto TcpTransport::doRequest(Message::Ptr msg) -> FutureMessage
{
    std::lock_guard lock { m_pending_requests_mtx };

    Request request;
    auto result = request.get_future();
    const auto msg_id = msg->id();
    m_pending_requests.insert(std::make_pair(msg_id, std::move(request)));
    m_pending_requests_ts.insert(std::make_pair(msg_id, Clock::now()));
    send(std::move(msg));
    return result;
}

auto TcpTransport::removeExpiredRequests() -> void
{
    std::lock_guard lock { m_pending_requests_mtx };

    std::vector<MessageId> ids;
    for (const auto& [id, ts] : m_pending_requests_ts)
    {
        const auto seconds_elapsed =
            std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - ts);
        if (seconds_elapsed > 30s)
            ids.push_back(id);
    }

    for (auto id : ids)
    {
        m_pending_requests_ts.erase(id);
        if (!m_pending_requests.contains(id))
            continue;
        try
        {
            auto& pending_request = m_pending_requests.at(id);
            try
            {
                failwith("Request is expired");
            }
            catch (...)
            {
                pending_request.set_exception(std::current_exception());
            }
            m_pending_requests.erase(id);
        }
        catch (const std::exception& ex)
        {
            // Do nothing
        }
    }
}

auto TcpTransport::writeMessage() -> void
{
    auto msg = m_outgoing_messages.pull();

    if (!msg) return;

    std::stringstream stream;
    auto context = SerializationContext::create<BinarySerializer>(stream);

    // hack: real message length will be written later
    m_outgoing_data_length = 0U;
    m_outgoing_data_length += static_cast<std::uint16_t>(binary::size(m_outgoing_data_length));
    m_outgoing_data_length += static_cast<std::uint16_t>(binary::size(*msg));

    serialize<BinarySerializer>(context, m_outgoing_data_length, "");
    serialize<BinarySerializer>(context, *msg, "");

    resizeOutgoingDataBuffer();
    const auto data = stream.str();
    std::copy(data.cbegin(), data.cend(), m_outgoing_data_buffer.get());

    auto handler =
        [this](const std::error_code& ec, std::size_t /*bytes_transferred*/) mutable
            {
                if (disconnected(ec)) return;

                if (ec)
                {
                    m_state = Service::State::StopPending;
                    return;
                }
                else
                {
                    writeMessage();
                }
            };

    boost::asio::async_write(m_socket,
        boost::asio::buffer(m_outgoing_data_buffer.get(), m_outgoing_data_length),
        boost::asio::transfer_all(),
        handler);
}

auto TcpTransport::readMessageLength() -> void
{
    auto handler =
        [this](const std::error_code& ec, std::size_t /*bytes_transferred*/) mutable
            {
                if (disconnected(ec)) return;

                if (ec)
                {
                    m_state = Service::State::StopPending;
                    return;
                }
                else
                {
                    resizeIncomingDataBuffer();
                    readMessage();
                }
            };

    boost::asio::async_read(m_socket,
        boost::asio::buffer(&m_incoming_data_length, sizeof(m_incoming_data_length)),
        boost::asio::transfer_all(),
        handler);
}

auto TcpTransport::readMessage() -> void
{
    auto handler =
        [this](const std::error_code& ec, std::size_t /*bytes_transferred*/) mutable
            {
                if (disconnected(ec)) return;

                if (ec)
                {
                    m_state = Service::State::StopPending;
                    return;
                }
                else
                {
                    onMessageRead();
                    readMessageLength();
                }
            };

    boost::asio::async_read(m_socket,
        boost::asio::buffer(m_incoming_data_buffer.get(), m_incoming_data_length),
        boost::asio::transfer_all(),
        handler);
}

auto TcpTransport::onMessageRead() -> void
{
    try
    {
        std::stringstream stream {
            std::string(m_incoming_data_buffer.get(),
                m_incoming_data_buffer.get() + m_incoming_data_length) };

        if (auto maybe_message = createMessageFromStream(stream))
        {
            auto& message = maybe_message.value();

            bool should_be_queued = true;
            if (message->hasField("srcMsgId"))
            {
                const auto src_msg_id = message->field<MessageId>("srcMsgId").get();
                std::lock_guard lock { m_pending_requests_mtx };
                if (m_pending_requests.contains(src_msg_id))
                {
                    should_be_queued = false;
                    auto& promise = m_pending_requests.at(src_msg_id);
                    promise.set_value(std::move(message));
                    m_pending_requests.erase(src_msg_id);
                    m_pending_requests_ts.erase(src_msg_id);
                }
            }

            if (should_be_queued)
            {
                m_incoming_messages.push(std::move(message));
            }
        }
    }
    catch (const std::exception& ex)
    {
        // Do nothing
    }
}

auto TcpTransport::createMessageFromStream(std::stringstream& stream) -> Maybe<Message::Ptr>
{
    auto context = SerializationContext::create<BinarySerializer>(stream);

    MessageType type {};
    deserialize<BinarySerializer>(context, type, "");

    auto& factory = MessageFactory::getInstance();
    if (!factory.hasDescriptor(type))
        return none;

    assert(m_session);
    auto message = factory.createMessage(type, *m_session);
    deserialize<BinarySerializer>(context, *message, "");

    return Maybe { std::move(message) };
}

auto TcpTransport::resizeIncomingDataBuffer() -> void
{
    m_incoming_data_buffer.reset(new char[m_incoming_data_length + 1]);
    m_incoming_data_buffer[m_incoming_data_length] = '\0';
}

auto TcpTransport::resizeOutgoingDataBuffer() -> void
{
    m_outgoing_data_buffer.reset(new char[m_outgoing_data_length]);
}

auto TcpTransport::disconnected(const std::error_code& ec) -> bool
{
    if (ec.value() == boost::asio::error::connection_refused || ec.value() == boost::asio::error::eof)
    {
        m_state = Service::State::StopPending;
        return true;
    }

    return false;
}

} // namespace isml
