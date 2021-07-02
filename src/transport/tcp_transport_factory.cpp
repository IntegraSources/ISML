/**
 * @file    tcp_transport_factory.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    06.03.2021
 */

#include <isml/transport/tcp_transport_factory.hpp>

ISML_DISABLE_WARNINGS_PUSH
#   include <boost/asio/ip/tcp.hpp>
ISML_DISABLE_WARNINGS_POP

#include <isml/transport/tcp_transport.hpp>

namespace isml {

TcpTransportFactory::TcpTransportFactory(boost::asio::io_context& ioc) noexcept
    : m_ioc(ioc)
{}

auto TcpTransportFactory::createTransport(const Url& url) noexcept -> Result<Transport::Ptr, std::error_code>
{
    boost::asio::ip::tcp::resolver resolver { m_ioc.get() };

    boost::system::error_code ec;
    auto it = resolver.resolve(url.hostname(), std::to_string(url.port()), ec);

    if (ec) return Failure { std::error_code(ec.value(), std::system_category()) };

    const auto& endpoint = it->endpoint();

    boost::asio::ip::tcp::socket socket { m_ioc.get() };
    socket.connect(endpoint, ec);

    if (ec) return Failure { std::error_code(ec.value(), std::system_category()) };

    std::unique_ptr<Transport> transport { new TcpTransport(std::move(socket)) };

    return Success { std::move(transport) };
}

auto TcpTransportFactory::supports(const std::string& protocol) const noexcept -> bool
{
    return protocol == "tcp";
}

} // namespace isml
