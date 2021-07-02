/**
 * @file    message_filter_chain.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    8/25/20
 */

#include <isml/message/message_filter_chain.hpp>
#include <isml/message/exceptions.hpp>

namespace isml {

auto MessageFilterChain::addFilter(MessageFilter::Ptr filter) -> MessageFilterChain&
{
    if (!filter)
    {
        throw InvalidArgumentException("Message filter instance is null pointer");
    }

    m_filters.push_back(std::move(filter));
    return *this;
}

auto MessageFilterChain::matches(const Message::Ptr& msg) -> bool
{
    for (auto& filter : m_filters)
    {
        if (!filter->matches(msg))
            return false;
    }

    return true;
}

} // namespace isml
