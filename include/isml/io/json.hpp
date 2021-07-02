/**
 * @file    json.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    21.01.2020
 */

#ifndef ISML_JSON_HPP
#define ISML_JSON_HPP

#include <ostream>
#include <istream>

#include <isml/types.hpp>

ISML_DISABLE_WARNINGS_PUSH
#   include <json/json.hpp>
ISML_DISABLE_WARNINGS_POP

namespace isml {

using JsonObject = Json::Value; ///< Json object type.
using JsonType = Json::ValueType; ///< Json value type.

namespace placeholder {

/**
 * @class  fake
 * @brief  Fake used as placeholder in commands which don't imply
 *         a payload storing.
 */

struct fake {};

} // namespace placeholder
} // namespace isml

#endif // ISML_JSON_HPP
