/**
 * @file    message_dispatcher.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    19/02/2020
 */

#include <isml/message/message_dispatcher.hpp>

#include <utility>
#include <algorithm>

#include <isml/session/session.hpp>

namespace isml {

MessageDispatcher::MessageDispatcher()
{
    onMessageRejected = [](Message::Ptr){};
}

auto MessageDispatcher::addHandler(MessageType type, MessageDispatcher::Handler handler) -> void
{
    m_handlers[type] = std::move(handler);
}

auto MessageDispatcher::hasHandler(MessageType type) const noexcept -> bool
{
    return m_handlers.contains(type);
}

auto MessageDispatcher::addDelegatedDispatcher(MessageDispatcher& dispatcher) -> void
{
    m_delegated_dispatchers.push_back(&dispatcher);
}

auto MessageDispatcher::filter() noexcept -> MessageFilterChain&
{
    return m_filter_chain;
}

auto MessageDispatcher::searchWithinDelegatedDispatchers(MessageType type) -> MessageDispatcher*
{
    const auto it =
        std::find_if(m_delegated_dispatchers.cbegin(), m_delegated_dispatchers.cend(),
            [=](const MessageDispatcher* dispatcher)
                {
                    return dispatcher->hasHandler(type);
                });

    return it != m_delegated_dispatchers.cend()
         ? *it
         : nullptr;
}

auto MessageDispatcher::dispatch(Message::Ptr msg) -> void
{
    doDispatch(std::move(msg));
}

auto MessageDispatcher::doDispatch(Message::Ptr msg) -> void
{
    try
    {
        if (!hasHandler(msg->type()))
        {
            if (auto delegate = searchWithinDelegatedDispatchers(msg->type()))
            {
                delegate->dispatch(std::move(msg));
                return;
            }

            return;
        }

        if (m_filter_chain.matches(msg))
        {
            auto& handler = m_handlers.at(msg->type());
            handler(std::move(msg));
            return;
        }

        onMessageRejected(std::move(msg));
    }
    catch (...)
    {
        // Do nothing
    }
}

} // namespace isml
