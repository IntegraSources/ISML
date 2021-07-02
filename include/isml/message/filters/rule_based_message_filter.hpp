/**
 * @file    rule_based_message_filter.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    8/25/20
 */

#ifndef ISML_RULE_BASED_MESSAGE_FILTER_HPP
#define ISML_RULE_BASED_MESSAGE_FILTER_HPP

#include <functional>
#include <vector>

#include <isml/message/message_filter.hpp>

namespace isml {

/**
 * @class   RuleBasedMessageFilter
 * @brief   Rule-based message filter.
 */

class RuleBasedMessageFilter : public MessageFilter
{
public:
    using Rule = std::function<bool(const Message::Ptr&)>;
    using RuleList = std::vector<Rule>;

public:
    RuleBasedMessageFilter() = default;
    explicit RuleBasedMessageFilter(Policy policy);

public:

    /**
     * @brief   Adds a rule that contains criteria to permit message
     *          to be passed.
     * @param   rule  Rule.
     */

    auto permit(Rule rule) -> void;

    /**
     * @brief   Adds a rule that contains criteria to forbid message
     *          to be passed.
     * @param   rule  Rule.
     */

    auto forbid(Rule rule) -> void;

    // Interface: MessageFilter
    auto matches(const Message::Ptr& msg) -> bool override;

protected:

    /**
     * @brief   Checks whether the message satisfies at least one rule from
     *          the specified ruleset.
     * @param   msg    Message.
     * @param   rules  Ruleset.
     *
     * @return  If message satisfies at least one rule - true, otherwise - false.
     */

    static auto satisfies(const Message::Ptr& msg, const RuleList& rules) -> bool;

protected:
    Policy   m_policy    {}; ///< Filter policy.
    RuleList m_permitted {}; ///< Permitted rules.
    RuleList m_forbidden {}; ///< Forbidden rules.
};

} // namespace isml

#endif // ISML_RULE_BASED_MESSAGE_FILTER_HPP
