/**
 * @file    string_utils.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    10/19/20
 */

#ifndef ISML_STRING_UTILS_HPP
#define ISML_STRING_UTILS_HPP

#include <string>
#include <sstream>
#include <algorithm>

namespace isml {

template<typename T, typename... Ts>
auto join(const std::string& separator, const T& string, const Ts&... strings) -> std::string
{
    std::stringstream ss;
    ss << string;
    ((ss << separator << strings), ...);
    return ss.str();
}

inline auto join(const std::string&) noexcept -> std::string
{
    return {};
}

template<typename String>
auto to_lower(String str) -> std::string
{
    std::transform(std::cbegin(str), std::cend(str),
        std::begin(str),
        [](unsigned char ch){ return std::tolower(ch); });
    return str;
}

} // namespace isml

#endif // ISML_STRING_UTILS_HPP
