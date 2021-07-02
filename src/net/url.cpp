/**
 * @file    url.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/10/20
 */

#include <isml/net/url.hpp>

#include <utility>

#include <isml/exceptions.hpp>
#include <isml/net/url_builder.hpp>
#include <isml/io/format.hpp>

namespace isml {

Url::Url(std::string protocol, std::string hostname)
    : m_protocol(std::move(protocol))
    , m_hostname(std::move(hostname))
    , m_port(0)
{
    if (m_protocol.empty())
        throw MalformedUrlException("Protocol is not specified");

    if (m_hostname.empty())
        throw MalformedUrlException("Hostname is not specified");
}

Url::Url(std::string protocol, std::string hostname, unsigned port)
    : m_protocol(std::move(protocol))
    , m_hostname(std::move(hostname))
    , m_port(port)
{
    if (m_protocol.empty())
        throw MalformedUrlException("Hostname is not specified");

    if (m_hostname.empty())
        throw MalformedUrlException("Hostname is not specified");

    if (!m_port)
        throw MalformedUrlException("Bad port number");
}

auto Url::protocol() const noexcept -> const std::string&
{
    return m_protocol;
}

auto Url::hostname() const noexcept -> const std::string&
{
    return m_hostname;
}

auto Url::port() const noexcept -> unsigned
{
    return m_port;
}

auto Url::path() const noexcept -> const std::string&
{
    return m_path;
}

auto Url::toString() const noexcept -> std::string
{
    auto str = fmt::format("{}://{}", m_protocol, m_hostname);

    if (m_port)
        str += fmt::format(":{}", m_port);

    if (!m_path.empty())
    {
        if (!m_path.starts_with("/")) str += "/";
        str += m_path;
    }

    if (!m_query.empty())
        str += fmt::format("/{}", m_query);

    if (!m_parameters.empty())
    {
        str += "?";
        auto it = m_parameters.cbegin();
        str += fmt::format("{}={}", it->first, it->second);
        it = std::next(it);
        for (; it != m_parameters.cend(); ++it)
        {
            str += "&";
            str += fmt::format("{}={}", it->first, it->second);
        }
    }

    return str;
}

auto Url::parse(const std::string& str) -> Url
{
    std::smatch match;
    if (!std::regex_match(str, match, s_regex))
        throw MalformedUrlException("Cannot parse string");

    UrlBuilder builder;

    // Protocol
    if (match.size() > 2)
        builder.setProtocol(match[2].str());

    // Hostname/Port
    if (match.size() > 4)
    {
        const auto hostport = match[4].str();
        const auto sep = hostport.find(':');
        if (sep == std::string::npos)
        {
            builder.setHostname(hostport);
        }
        else
        {
            builder.setHostname(hostport.substr(0, sep));
            if (sep < hostport.size())
            {
                const auto port = hostport.substr(sep + 1, hostport.size() - sep - 1);
                try
                {
                    builder.setPort(std::stoi(port));
                }
                catch (...)
                {
                    throw MalformedUrlException("Invalid port value");
                }
            }
        }
    }

    // Path
    if (match.size() > 5)
        builder.setPath(match[5].str());

    // Query
    if (match.size() > 7)
    {
        auto parameters = match[7].str();

        std::smatch sub_match;
        while (std::regex_search(parameters, sub_match, s_param_regex))
        {
            const auto kv = sub_match[0].str();

            const auto sep = kv.find("=");
            if (sep == std::string::npos)
            {
                parameters = sub_match.suffix();
                continue;
            }

            if (sep < kv.size())
            {
                const auto key = kv.substr(0, sep);
                const auto val = kv.substr(sep + 1, kv.size() - sep - 1);
                builder.addParameter(key, val);
            }

            parameters = sub_match.suffix();
        }
    }

    return builder.build();
}

auto Url::clearParameters() -> void
{
    m_parameters.clear();
}

auto Url::clear() -> void
{
    *this = {};
}

} // namespace isml
