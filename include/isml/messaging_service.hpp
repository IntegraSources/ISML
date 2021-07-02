/**
 * @file    messaging_service.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    03.03.2021
 */

#ifndef ISML_MESSAGING_SERVICE_HPP
#define ISML_MESSAGING_SERVICE_HPP

#include <future>
#include <system_error>
#include <functional>

ISML_DISABLE_WARNINGS_PUSH
#   include <boost/asio/io_context.hpp>
ISML_DISABLE_WARNINGS_POP

#include <isml/base/result.hpp>

#include <isml/transport/transport_registry.hpp>

#include <isml/session/session_manager.hpp>

#include <isml/service/service.hpp>

namespace isml {

class MessagingService : public Service
{
public:
    MessagingService() = default;

public:
    auto context() noexcept -> boost::asio::io_context&;

    auto transportRegistry() noexcept -> TransportRegistry&;

    auto sessionManager() noexcept -> SessionManager&;

    auto connect(const Url& url) -> Result<Session::Ptr, std::error_code>;

private:
    auto doStart() -> void override;
    auto doInit() -> void override;
    auto doStop() -> void override;

protected:
    boost::asio::io_context m_ioc                {};
    std::future<void>       m_io                 {};
    TransportRegistry       m_transport_registry {};
    SessionManager          m_session_manager    {};
};

} // namespace isml

#endif // ISML_MESSAGING_SERVICE_HPP
