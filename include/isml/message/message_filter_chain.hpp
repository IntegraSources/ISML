/**
 * @file    message_filter_chain.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    8/25/20
 */

#ifndef ISML_MESSAGE_FILTER_CHAIN_HPP
#define ISML_MESSAGE_FILTER_CHAIN_HPP

#include <vector>
#include <utility>
#include <type_traits>

#include <isml/message/message_filter.hpp>

namespace isml {

/**
 * @class   MessageFilterChain
 * @brief   Message filter chain is a special type of message filter that
 *          aggregates other message filters into a single filter.
 */

class MessageFilterChain final
    : public MessageFilter
{
private:
    using Filters = std::vector<MessageFilter::Ptr>;

public:
    MessageFilterChain() = default;

    // Non-copyable
    MessageFilterChain(const MessageFilterChain&) = delete;
    auto operator=(const MessageFilterChain&) -> MessageFilterChain& = delete;

public:

    /**
     * @brief   Creates a filter of the specified type and put it to the end
     *          of the chain.
     *
     * @tparam  Filter  The filter type.
     * @tparam  Args    The filter constructor argument types.
     *
     * @param   args    Arguments for a filter constructor.
     *
     * @return  Reference to itself.
     */

    template<typename Filter, typename... Args>
        requires std::is_base_of_v<MessageFilter, Filter>
    auto addFilter(Args&&... args) -> MessageFilterChain&;

    /**
     * @brief   Adds to the chain already create message filter.
     * @param   filter  Filter.
     *
     * @return  Reference to itself.
     *
     * @throw   InvalidArgumentException - will be thrown if passed message filter
     *          instance is a null pointer.
     */

    auto addFilter(MessageFilter::Ptr filter) -> MessageFilterChain&;

    // Interface: MessageFilter
    auto matches(const Message::Ptr& msg) -> bool override;

private:
    Filters m_filters {}; ///< Filter chain.
};

template<typename Filter, typename... Args>
    requires std::is_base_of_v<MessageFilter, Filter>
auto MessageFilterChain::addFilter(Args&&... args) -> MessageFilterChain&
{
    auto filter = std::make_unique<Filter>(std::forward<Args>(args)...);
    addFilter(std::move(filter));
    return *this;
}

} // namespace isml

#endif // ISML_MESSAGE_FILTER_CHAIN_HPP
