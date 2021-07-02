/**
 * @file    service.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    05/03/2020
 */

#include <isml/service/service.hpp>

namespace isml {

auto Service::start() -> void
{
    doStart();
}

auto Service::init() -> void
{
    doInit();
}

auto Service::stop() -> void
{
    doStop();
}

auto Service::state() const noexcept -> Service::State
{
    return m_state;
}

auto Service::started() const noexcept -> bool
{
    return m_state == State::Started;
}

auto Service::stopped() const noexcept -> bool
{
    return m_state == State::Stopped;
}

} // namespace isml
