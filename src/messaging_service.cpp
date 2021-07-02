/**
 * @file    messaging_service.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    03.03.2021
 */

#include <isml/messaging_service.hpp>

#include <chrono>

using namespace std::chrono_literals;

namespace isml {

auto MessagingService::context() noexcept -> boost::asio::io_context&
{
    return m_ioc;
}

auto MessagingService::transportRegistry() noexcept -> TransportRegistry&
{
    return m_transport_registry;
}

auto MessagingService::sessionManager() noexcept -> SessionManager&
{
    return m_session_manager;
}

auto MessagingService::connect(const Url& url) -> Result<Session::Ptr, std::error_code>
{
    try
    {
        auto maybe_factory = m_transport_registry.getFactory(url.protocol());
        if (maybe_factory.isNone())
            return Failure { std::make_error_code(std::errc::protocol_not_supported) };

        auto transport_res = maybe_factory->createTransport(url);
        if (!transport_res)
            return Failure { transport_res.error() };

        auto session =
            m_session_manager.createSession(std::move(transport_res.value()));

        return Success { std::move(session) };
    }
    catch (...)
    {
        // Internal error
        return Failure { std::make_error_code(static_cast<std::errc>(0xFF)) };
    }
}

auto MessagingService::doStart() -> void
{
    auto handler = [this]
        {
            m_state = State::Started;
            while (m_state != State::StopPending)
            {
                try
                {
                    boost::asio::executor_work_guard<decltype(m_ioc.get_executor())> guard { m_ioc.get_executor() };
                    m_ioc.run();
                }
                catch (...)
                {
                    // Do nothing
                }
            }
            m_state = State::Stopped;
        };

    m_state = State::StartPending;
    m_io = std::async(std::launch::async, handler);
}

auto MessagingService::doInit() -> void
{}

auto MessagingService::doStop() -> void
{
    try
    {
        m_state = State::StopPending;
        m_session_manager.terminateAll();
        m_ioc.stop();
        m_io.wait();
    }
    catch (const std::exception& ex)
    {
        // TODO: handle error
    }
}

} // namespace isml
