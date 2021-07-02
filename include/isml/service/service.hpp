/**
 * @file    service.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    05/03/2020
 */

#ifndef ISML_SERVICE_HPP
#define ISML_SERVICE_HPP

#include <atomic>
#include <memory>

namespace isml {

/**
 * @class  Service
 * @brief  Serves as a base class for all services.
 */

class Service
{
public:

    /**
     * @class  Mode
     * @brief  Defines values that specify a service state.
     */

    enum class State
    {
        StartPending,
        Started,
        StopPending,
        Stopped
    };

    using Ptr = std::shared_ptr<Service>;

public:
    virtual ~Service() = default;

    // non-copyable
    Service(const Service&) = delete;
    auto operator=(const Service&) -> Service& = delete;

protected:
    Service() = default;

public:
    auto start() -> void;
    auto init() -> void;
    auto stop() -> void;

    auto state() const noexcept -> State;

    auto started() const noexcept -> bool;

    auto stopped() const noexcept -> bool;

private:
    virtual auto doStart() -> void = 0;
    virtual auto doInit() -> void = 0;
    virtual auto doStop() -> void = 0;

protected:
    std::atomic<State> m_state = State::Stopped;
};

} // namespace isml

#endif // ISML_SERVICE_HPP
