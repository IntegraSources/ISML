/**
 * @file    interrupter.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    13/02/2020
 */

#ifndef ISML_SIGNAL_INTERCEPTOR_HPP
#define ISML_SIGNAL_INTERCEPTOR_HPP

#include <unordered_map>
#include <functional>

ISML_DISABLE_WARNINGS_PUSH
#   include <boost/asio/io_context.hpp>
#   include <boost/asio/signal_set.hpp>
ISML_DISABLE_WARNINGS_POP

namespace isml {

/**
 * @class  SignalInterceptor
 * @brief  Intercepts OS signals and process them with user-defined handlers.
 */

class SignalInterceptor
{
public:
    using Signal = int;                                               ///< Signal type.
    using SignalHandler = std::function<void()>;                      ///< Signal handler type.
    using SignalHandlers = std::unordered_map<Signal, SignalHandler>; ///< Signal handler map type.

public:
    explicit SignalInterceptor(boost::asio::io_context& ctx);
    virtual ~SignalInterceptor() = default;

public:

    /**
     * Add user-defined handler for specified signal.
     * @param  signal   Signal.
     * @param  handler  Signal handler.
     */

    auto addHandler(Signal signal, SignalHandler handler) -> void;

    /**
     * Starts intercepting in asynchronous mode.
     */

    auto run() -> void;

    /**
     * Cancel intercepting.
     */

    auto cancel() -> void;

protected:
    SignalHandlers          m_signal_handlers;
    boost::asio::signal_set m_signal_set;
};

} // namespace isml

#endif // ISML_SIGNAL_INTERCEPTOR_HPP
