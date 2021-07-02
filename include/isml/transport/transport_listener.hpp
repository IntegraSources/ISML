/**
 * @file    transport_listener.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/16/20
 */

#ifndef ISML_TRANSPORT_LISTENER_HPP
#define ISML_TRANSPORT_LISTENER_HPP

#include <system_error>
#include <isml/service/service.hpp>

namespace isml {

class Transport;

/**
 * @class   TransportListener
 * @brief   A listener interface for monitoring events of a particular type
 *          of @a Transport instance.
 */

class TransportListener
{
public:
    using State = Service::State;

public:
    virtual ~TransportListener() = default;

public:
    virtual auto onStateChanged(Transport& transport, State from, State to) -> void = 0;
    virtual auto onErrorOccurred(Transport& transport, const std::error_code& ec) -> void = 0;
};

} // namespace isml

#endif // ISML_TRANSPORT_LISTENER_HPP
