/**
 * @file    url_builder.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/10/20
 */

#include <isml/net/url_builder.hpp>

#include <utility>

#include <isml/exceptions.hpp>

namespace isml {

auto UrlBuilder::setProtocol(std::string protocol) -> UrlBuilder&
{
    m_url.m_protocol = std::move(protocol);
    return *this;
}

auto UrlBuilder::setHostname(std::string hostname) -> UrlBuilder&
{
    m_url.m_hostname = std::move(hostname);
    return *this;
}

auto UrlBuilder::setPort(unsigned port) -> UrlBuilder&
{
    m_url.m_port = port;
    return *this;
}

auto UrlBuilder::setPath(std::string path) -> UrlBuilder&
{
    m_url.m_path = std::move(path);
    return *this;
}

auto UrlBuilder::setQuery(std::string query) -> UrlBuilder&
{
    m_url.m_query = std::move(query);
    return *this;
}

auto UrlBuilder::build() const -> Url
{
    if (m_url.m_protocol.empty())
        throw MalformedUrlException("Protocol is not specified");

    if (m_url.m_hostname.empty())
        throw MalformedUrlException("Protocol is not specified");

    return m_url;
}

} // namespace isml
