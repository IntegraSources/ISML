/**
 * @file    message_filter.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    8/25/20
 */

#ifndef ISML_MESSAGE_FILTER_HPP
#define ISML_MESSAGE_FILTER_HPP

#include <memory>

#include <isml/message/message.hpp>

namespace isml {

/**
 * @class   MessageFilter
 * @brief   Serves as a base class for classes intended for message filtering.
 */

class MessageFilter
{
public:
    using Ptr = std::unique_ptr<MessageFilter>;

public:

    /**
     * @enum    Policy
     * @brief   Defines filter policies.
     */

    enum class Policy
    {
        ExactMatchingToAccessList,
        PermitAllExceptForbidden,
        ForbidAllExceptPermitted,
    };

public:
    virtual ~MessageFilter() = default;

public:

    /**
     * @brief   Checks whether a message satisfies the criteria of a filter.
     * @param   msg  Message.
     *
     * @return  If message satisfies the filter criteria - return true,
     *          otherwise - false.
     */

    virtual auto matches(const Message::Ptr& msg) -> bool = 0;
};

} // namespace isml

#endif // ISML_MESSAGE_FILTER_HPP
