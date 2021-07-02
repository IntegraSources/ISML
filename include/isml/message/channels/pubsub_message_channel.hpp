/**
 * @file    pubsub_message_channel.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    5/19/20
 */

#ifndef ISML_PUBSUB_MESSAGE_CHANNEL_HPP
#define ISML_PUBSUB_MESSAGE_CHANNEL_HPP

#include <unordered_map>
#include <mutex>
#include <functional>

#include <isml/message/channels/subscribable_channel.hpp>

namespace isml {

class PubSubMessageChannel : public SubscribableChannel
{
public:
    enum class ReasonForLeaving
    {
        Unknown,
        SubscriberTransportHasBeenStopped,
        SubscriberLeftChannelOnHisOwn,
    };

public:
    using SubscriberId = SessionId;
    using Subscriber = Session::Ptr;
    using Subscribers = std::unordered_map<SubscriberId, Subscriber>;

    using UnsubscribeHandler = std::function<void(const Subscriber&, ReasonForLeaving)>;

public:
    PubSubMessageChannel();

    PubSubMessageChannel(const PubSubMessageChannel&) = delete;
    auto operator=(const PubSubMessageChannel&) -> PubSubMessageChannel& = delete;

    PubSubMessageChannel(PubSubMessageChannel&&) = delete;
    auto operator=(PubSubMessageChannel&&) -> PubSubMessageChannel& = delete;

public:
    auto publishWithProducer(const std::function<Message::Ptr(Subscriber& subscriber)>& producer) -> void;

protected:
    void internal_send(Message::Ptr msg) override;
    auto internal_subscribe(Subscriber& subscriber) -> SubscribeResult override;
    auto internal_unsubscribe(const Subscriber& subscriber) -> UnsubscribeResult override;

private:
    auto helper_unsubscribe(SubscriberId id, ReasonForLeaving reason) -> UnsubscribeResult;

public:
    UnsubscribeHandler onUnsubscribed {};  ///< Will be invoked as soon as the subscriber is unsubscribed.

protected:
    Subscribers m_subscribers       {}; ///< Subscriber list.
    std::mutex  m_subscribers_guard {}; ///< Subscriber list guard.
};

} // namespace isml

#endif // ISML_PUBSUB_MESSAGE_CHANNEL_HPP
