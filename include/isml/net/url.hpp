/**
 * @file    url.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/10/20
 */

#ifndef ISML_URL_HPP
#define ISML_URL_HPP

#include <string>
#include <unordered_map>
#include <type_traits>
#include <regex>

namespace isml {

class Url
{
public:
    using Parameters = std::unordered_map<std::string, std::string>;

public:
    Url() = default;
    Url(std::string protocol, std::string hostname);
    Url(std::string protocol, std::string hostname, unsigned port);
    virtual ~Url() = default;

public:
    auto protocol() const noexcept -> const std::string&;

    auto hostname() const noexcept -> const std::string&;

    auto port() const noexcept -> unsigned;

    auto path() const noexcept -> const std::string&;

    auto addParameter(const std::string& name, const std::string& value) -> Url&;

    auto toString() const noexcept -> std::string;

    static auto parse(const std::string& str) -> Url;

protected:
    auto clearParameters() -> void;
    auto clear() -> void;

protected:
    std::string m_protocol      {};
    std::string m_hostname      {};
    unsigned    m_port          {};
    std::string m_path          {};
    std::string m_query         {};
    Parameters  m_parameters    {};

    static inline const std::regex s_regex { "^(([^:/?#]+):)?(\\/\\/([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?" };
    static inline const std::regex s_param_regex { "([a-zA-Z0-9]+\\=[a-zA-Z0-9]+)+" };

    friend class UrlBuilder;
};

inline auto Url::addParameter(const std::string& name, const std::string& value) -> Url&
{
    m_parameters[name] = value;
    return *this;
}

} // namespace isml

#endif // ISML_URL_HPP
