/**
 * @file    interrupter.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    13/02/2020
 */

#include <isml/sys/signal_interceptor.hpp>
#include <isml/exceptions.hpp>

namespace isml {

SignalInterceptor::SignalInterceptor(boost::asio::io_context& ctx)
    : m_signal_handlers()
    , m_signal_set(ctx)
{}

auto SignalInterceptor::run() -> void
{
    auto handler =
        [this](const std::error_code& ec, Signal signal)
            {
                if (!ec)
                {
                    if (auto it = m_signal_handlers.find(signal); it != m_signal_handlers.end())
                    {
                        try
                        {
                            std::invoke(it->second);
                        }
                        catch (...)
                        {}
                    }
                }
            };

    m_signal_set.async_wait(handler);
}

auto SignalInterceptor::addHandler(Signal signal, SignalHandler handler) -> void
{
    try
    {
        m_signal_set.add(signal);
        m_signal_handlers[signal] = std::move(handler);
    }
    catch (const std::system_error& err)
    {
        throw SystemException("Cannot set signal handler", err.code());
    }
}

auto SignalInterceptor::cancel() -> void
{
    try
    {
        m_signal_set.cancel();
    }
    catch (const std::system_error& ex)
    {
        throw SystemException("Cannot cancel signal interception", ex.code());
    }
}

} // namespace isml
