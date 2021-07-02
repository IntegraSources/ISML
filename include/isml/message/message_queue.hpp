/**
 * @file    message_queue.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/02/2020
 */

#ifndef ISML_MESSAGE_QUEUE_HPP
#define ISML_MESSAGE_QUEUE_HPP

#include <queue>
#include <mutex>

#include <isml/message/message.hpp>

namespace isml {

class MessageQueue
{
public:
    using Queue = std::queue<Message::Ptr>;

public:
    MessageQueue();
    virtual ~MessageQueue() = default;

public:
    auto push(Message::Ptr msg) noexcept -> void;
    auto pull() noexcept -> Message::Ptr;
    auto size() const noexcept -> std::size_t;

protected:
    virtual auto internal_push(Message::Ptr msg) noexcept -> void;
    virtual auto internal_pull() noexcept -> Message::Ptr;
    virtual auto internal_size() const noexcept -> std::size_t;

protected:
    Queue m_queue;
};

class ConcurrentMessageQueue : public MessageQueue
{
public:
    ConcurrentMessageQueue();

protected:
    auto internal_push(Message::Ptr msg) noexcept -> void override;
    auto internal_pull() noexcept -> Message::Ptr override;
    auto internal_size() const noexcept -> std::size_t override;

private:
    mutable std::mutex m_queue_guard;
};

} // namespace isml

#endif // ISML_MESSAGE_QUEUE_HPP
