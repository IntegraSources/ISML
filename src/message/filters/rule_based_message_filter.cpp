/**
 * @file    rule_based_message_filter.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    8/25/20
 */

#include <isml/message/filters/rule_based_message_filter.hpp>

#include <utility>
#include <algorithm>

namespace isml {

RuleBasedMessageFilter::RuleBasedMessageFilter(MessageFilter::Policy policy)
    : m_policy(policy)
{}

auto RuleBasedMessageFilter::permit(Rule rule) -> void
{
    m_permitted.push_back(std::move(rule));
}

auto RuleBasedMessageFilter::forbid(Rule rule) -> void
{
    m_forbidden.push_back(std::move(rule));
}

ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic push)
ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wreturn-type")
bool RuleBasedMessageFilter::matches(const Message::Ptr& msg)
{
    switch (m_policy)
    {
        case Policy::PermitAllExceptForbidden:
            return !satisfies(msg, m_forbidden);
        case Policy::ForbidAllExceptPermitted:
            return satisfies(msg, m_permitted);
        case Policy::ExactMatchingToAccessList:
            return satisfies(msg, m_permitted)
                && !satisfies(msg, m_forbidden);
    }
}
ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic pop)

auto RuleBasedMessageFilter::satisfies(const Message::Ptr& msg, const RuleList& rules) -> bool
{
    return std::any_of(rules.cbegin(), rules.cend(),
        [&](const auto& rule){ return rule(msg); });
}

} // namespace isml
