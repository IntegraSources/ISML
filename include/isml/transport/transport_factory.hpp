/**
 * @file    transport_factory.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/10/20
 */

#ifndef ISML_TRANSPORT_FACTORY_HPP
#define ISML_TRANSPORT_FACTORY_HPP

#include <string>
#include <system_error>

#include <isml/base/result.hpp>
#include <isml/net/url.hpp>
#include <isml/transport/transport.hpp>

namespace isml {

class TransportFactory
{
public:
    TransportFactory() = default;

    virtual ~TransportFactory() = default;

public:
    virtual auto createTransport(const Url& url) noexcept -> Result<Transport::Ptr, std::error_code> = 0;

    virtual auto supports(const std::string& protocol) const noexcept -> bool = 0;
};

} // namespace isml

#endif // ISML_TRANSPORT_FACTORY_HPP
