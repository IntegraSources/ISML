/**
 * @file    predicates.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/1/20
 */

#ifndef ISML_PREDICATES_HPP
#define ISML_PREDICATES_HPP

namespace isml {
namespace predicates {

inline auto is_equal =
    []<typename T>(const T& lhs) -> decltype(auto)
        {
            return [&lhs](const T& rhs) -> bool { return lhs == rhs; };
        };

} // namespace predicates
} // namespace isml

#endif // ISML_PREDICATES_HPP
