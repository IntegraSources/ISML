/**
 * @file    subscribable_channel.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    5/19/20
 */

#ifndef ISML_SUBSCRIBABLE_CHANNEL_HPP
#define ISML_SUBSCRIBABLE_CHANNEL_HPP

#include <isml/message/message_channel.hpp>
#include <isml/session/session.hpp>

namespace isml {

/**
 * @class   SubscribableChannel
 * @brief   Serves as a base class for all message channels of publish-subscribe
 *          type.
 */

class SubscribableChannel : public MessageChannel
{
public:

    /**
     * @enum    SubscribeResult
     * @brief   Specifies subscribe result statuses.
     */

    enum class SubscribeResult { Done, AlreadySubscribed };

    /**
     * @enum    UnsubscribeResult
     * @brief   Specifies unsubscribe result statuses.
     */

    enum class UnsubscribeResult { Done, NotFound };

public:
    SubscribableChannel(const SubscribableChannel&) = delete;
    auto operator=(const SubscribableChannel&) -> SubscribableChannel& = delete;

    SubscribableChannel(SubscribableChannel&&) = default;
    auto operator=(SubscribableChannel&&) -> SubscribableChannel& = delete;

protected:
    SubscribableChannel() = default;

public:
    auto subscribe(Session::Ptr& subscriber) -> SubscribeResult;
    auto unsubscribe(const Session::Ptr& subscriber) -> UnsubscribeResult;

protected:
    virtual auto internal_subscribe(Session::Ptr& subscriber) -> SubscribeResult = 0;
    virtual auto internal_unsubscribe(const Session::Ptr& subscriber) -> UnsubscribeResult = 0;
};

inline auto SubscribableChannel::subscribe(Session::Ptr& subscriber) -> SubscribeResult
{ return internal_subscribe(subscriber); }

inline auto SubscribableChannel::unsubscribe(const Session::Ptr& subscriber) -> UnsubscribeResult
{ return internal_unsubscribe(subscriber); }

} // namespace isml

#endif // ISML_SUBSCRIBABLE_CHANNEL_HPP
