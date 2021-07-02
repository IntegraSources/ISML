/**
 * @file    message_dispatcher.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    19/02/2020
 */

#ifndef ISML_MESSAGE_DISPATCHER_HPP
#define ISML_MESSAGE_DISPATCHER_HPP

#include <functional>
#include <unordered_map>
#include <vector>

#include <isml/base_types.hpp>

#include <isml/message/message.hpp>
#include <isml/message/message_filter_chain.hpp>

namespace isml {

/**
 * @class   MessageDispatcher
 * @brief   Consumes messages and distribute them to performers.
 */

class MessageDispatcher
{
public:
    using Handler = std::function<void(Message::Ptr)>;
    using HanlderMap = std::unordered_map<MessageType, Handler>;
    using DelegatedDispatchers = std::vector<MessageDispatcher*>;
    using MessageHandler = std::function<void(Message::Ptr)>;

public:
    MessageDispatcher();
    virtual ~MessageDispatcher() = default;

    // Non-copyable
    MessageDispatcher(const MessageDispatcher&) = delete;
    auto operator=(const MessageDispatcher&) -> MessageDispatcher& = delete;

    // Non-movable
    MessageDispatcher(MessageDispatcher&&) = delete;
    auto operator=(MessageDispatcher&&) -> MessageDispatcher& = delete;

public:
    auto dispatch(Message::Ptr msg) -> void;

    auto addHandler(MessageType type, Handler handler) -> void;

    auto hasHandler(MessageType type) const noexcept -> bool;

    auto addDelegatedDispatcher(MessageDispatcher& dispatcher) -> void;

    auto filter() noexcept -> MessageFilterChain&;

protected:
    auto searchWithinDelegatedDispatchers(MessageType type) -> MessageDispatcher*;

private:
    virtual auto doDispatch(Message::Ptr msg) -> void;

public:
    MessageHandler onMessageRejected {};

protected:
    HanlderMap           m_handlers              {}; ///< Handler map.
    DelegatedDispatchers m_delegated_dispatchers {}; ///< Delegated dispatchers.
    MessageFilterChain   m_filter_chain          {}; ///< Message filter chain.
};

} // namespace isml

#endif // ISML_MESSAGE_DISPATCHER_HPP
