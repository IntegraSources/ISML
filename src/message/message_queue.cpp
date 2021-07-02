/**
 * @file    message_queue.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/02/2020
 */

#include <isml/message/message_queue.hpp>

namespace isml {

MessageQueue::MessageQueue()
    : m_queue()
{}

auto MessageQueue::push(Message::Ptr msg) noexcept -> void
{
    internal_push(std::move(msg));
}

auto MessageQueue::pull() noexcept -> Message::Ptr
{
    return internal_pull();
}

auto MessageQueue::size() const noexcept -> std::size_t
{
    return internal_size();
}

auto MessageQueue::internal_push(Message::Ptr msg) noexcept -> void
{
    m_queue.push(std::move(msg));
}

auto MessageQueue::internal_pull() noexcept -> Message::Ptr
{
    if (!m_queue.empty())
    {
        auto message = std::move(m_queue.front());
        m_queue.pop();
        return message;
    }

    return nullptr;
}

auto MessageQueue::internal_size() const noexcept -> std::size_t
{
    return m_queue.size();
}

ConcurrentMessageQueue::ConcurrentMessageQueue()
    : MessageQueue()
    , m_queue_guard()
{}

auto ConcurrentMessageQueue::internal_push(Message::Ptr msg) noexcept -> void
{
    std::lock_guard lock { m_queue_guard };
    MessageQueue::internal_push(std::move(msg));
}

auto ConcurrentMessageQueue::internal_pull() noexcept -> Message::Ptr
{
    std::lock_guard lock { m_queue_guard };
    return MessageQueue::internal_pull();
}

auto ConcurrentMessageQueue::internal_size() const noexcept -> std::size_t
{
    std::lock_guard lock { m_queue_guard };
    return MessageQueue::internal_size();
}

} // namespace isml
