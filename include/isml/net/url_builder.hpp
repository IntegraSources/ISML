/**
 * @file    url_builder.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/10/20
 */

#ifndef ISML_URL_BUILDER_HPP
#define ISML_URL_BUILDER_HPP

#include <string>

#include <isml/net/url.hpp>

namespace isml {

/**
 * @class   UrlBuilder
 * @brief   An URL builder.
 */

class UrlBuilder
{
public:
    UrlBuilder() = default;
    virtual ~UrlBuilder() = default;

public:
    auto setProtocol(std::string protocol) -> UrlBuilder&;
    auto setHostname(std::string hostname) -> UrlBuilder&;
    auto setPort(unsigned port) -> UrlBuilder&;
    auto setPath(std::string path) -> UrlBuilder&;
    auto setQuery(std::string query) -> UrlBuilder&;

    template<typename T>
    auto addParameter(const std::string& name, const T& value) -> UrlBuilder&;

    auto build() const -> Url;

protected:
    Url m_url {};
};

template<typename T>
inline auto UrlBuilder::addParameter(const std::string& name, const T& value) -> UrlBuilder&
{
    m_url.addParameter(name, value);
    return *this;
}

} // namespace isml

#endif // ISML_URL_BUILDER_HPP
