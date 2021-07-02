/**
 * @file    session_base.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    7/23/20
 */

#ifndef ISML_SESSION_BASE_HPP
#define ISML_SESSION_BASE_HPP

#include <string>

#include <isml/utility/property_set.hpp>

namespace isml {

/**
 * @class   SessionBase
 * @brief   Serves as a base class for classes representing session.
 */

class SessionBase
{
public:
    using Properties = PropertySet<std::string>;

public:
    virtual ~SessionBase() = default;
};

} // namespace isml

#endif // ISML_SESSION_BASE_HPP
