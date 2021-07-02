/**
 * @file    tcp_transport_factory.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    06.03.2021
 */

#ifndef ISML_TCP_TRANSPORT_FACTORY_HPP
#define ISML_TCP_TRANSPORT_FACTORY_HPP

#include <functional>

ISML_DISABLE_WARNINGS_PUSH
#   include <boost/asio/io_context.hpp>
ISML_DISABLE_WARNINGS_POP

#include <isml/transport/transport_factory.hpp>

namespace isml {

class TcpTransportFactory : public TransportFactory
{
public:
    TcpTransportFactory() = delete;
    explicit TcpTransportFactory(boost::asio::io_context& ioc) noexcept;

public:
    auto createTransport(const Url& url) noexcept -> Result<Transport::Ptr, std::error_code> override;
    auto supports(const std::string& protocol) const noexcept -> bool override;

protected:
    std::reference_wrapper<boost::asio::io_context> m_ioc;
};

} // namespace isml

#endif // ISML_TCP_TRANSPORT_FACTORY_HPP
