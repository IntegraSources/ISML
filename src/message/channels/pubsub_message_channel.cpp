/**
 * @file    pubsub_message_channel.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    5/19/20
 */

#include <isml/message/channels/pubsub_message_channel.hpp>

#include <vector>

namespace isml {

static constexpr auto handler_stub1 = [](const Session::Ptr&, PubSubMessageChannel::ReasonForLeaving) {};

using SubscriberIds = std::vector<SessionId>;

PubSubMessageChannel::PubSubMessageChannel()
    : m_subscribers()
    , m_subscribers_guard()
{
    onUnsubscribed = handler_stub1;
}

auto PubSubMessageChannel::publishWithProducer(const std::function<Message::Ptr(Subscriber&)>& producer) -> void
{
    std::lock_guard lock { m_subscribers_guard };

    SubscriberIds broken_subscriber_ids;
    for (auto& [subscriber_id, subscriber] : m_subscribers)
    {
        try
        {
            if (subscriber->active())
            {
                subscriber->send(producer(subscriber));
                continue;
            }

            broken_subscriber_ids.push_back(subscriber_id);
        }
        catch (const std::exception&)
        {
            broken_subscriber_ids.push_back(subscriber_id);
        }
    }

    for (const auto subscriber_id : broken_subscriber_ids)
    {
        helper_unsubscribe(subscriber_id, ReasonForLeaving::SubscriberTransportHasBeenStopped);
    }
}

void PubSubMessageChannel::internal_send(Message::Ptr msg)
{
    std::lock_guard lock { m_subscribers_guard };

    SubscriberIds broken_subscriber_ids;
    for (auto& [subscriber_id, subscriber] : m_subscribers)
    {
        try
        {
            if (subscriber->active())
            {
                subscriber->send(msg->clone());
                continue;
            }

            broken_subscriber_ids.push_back(subscriber_id);
        }
        catch (const std::exception&)
        {
            broken_subscriber_ids.push_back(subscriber_id);
        }
    }

    for (const auto subscriber_id : broken_subscriber_ids)
    {
        helper_unsubscribe(subscriber_id, ReasonForLeaving::SubscriberTransportHasBeenStopped);
    }
}

auto PubSubMessageChannel::internal_subscribe(Session::Ptr& subscriber) -> SubscribeResult
{
    std::lock_guard lock { m_subscribers_guard };

    if (!m_subscribers.contains(subscriber->id()))
    {
        m_subscribers.insert(
            std::make_pair(subscriber->id(),
                           subscriber->shared_from_this()));

        return SubscribeResult::Done;
    }
    return SubscribeResult::AlreadySubscribed;
}

auto PubSubMessageChannel::internal_unsubscribe(const Session::Ptr& subscriber) -> UnsubscribeResult
{
    std::lock_guard lock { m_subscribers_guard };
    return helper_unsubscribe(subscriber->id(), ReasonForLeaving::SubscriberLeftChannelOnHisOwn);
}

auto PubSubMessageChannel::helper_unsubscribe(SubscriberId id, ReasonForLeaving reason) -> UnsubscribeResult
{
    if (m_subscribers.contains(id))
    {
        const auto&& subscriber = std::move(m_subscribers.at(id));
        m_subscribers.erase(id);
        onUnsubscribed(subscriber, reason);
        return UnsubscribeResult::Done;
    }
    return UnsubscribeResult::NotFound;
}

} // namespace isml
