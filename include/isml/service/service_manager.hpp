/**
 * @file    service_manager.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    05/03/2020
 */

#ifndef ISML_SERVICE_MANAGER_HPP
#define ISML_SERVICE_MANAGER_HPP

#include <unordered_map>
#include <mutex>
#include <typeindex>

#include <isml/exceptions.hpp>

#include <isml/service/service.hpp>

namespace isml {

/**
 * @class  ServiceManager
 * @brief  Manages services and provides access to them.
 */

class ServiceManager
{
protected:
    using ServiceType = std::type_index;
    using Services = std::unordered_map<ServiceType, Service::Ptr>;

public:
    virtual ~ServiceManager() = default;

    ServiceManager(const ServiceManager&) = delete;
    auto operator=(const ServiceManager&) -> ServiceManager& = delete;

protected:
    ServiceManager() = default;

public:
    template<typename    ConcreteService,
             typename... Args>
        requires std::is_base_of_v<Service, ConcreteService>
    auto publish(Args&&... args) -> std::shared_ptr<ConcreteService>;

    template<typename ConcreteService>
        requires std::is_base_of_v<Service, ConcreteService>
    auto terminate() -> bool;

    auto terminate() -> void;

    template<typename ConcreteService>
        requires std::is_base_of_v<Service, ConcreteService>
    auto getService() noexcept -> std::shared_ptr<ConcreteService>;

    static auto getInstance() noexcept -> ServiceManager&;

protected:
    template<typename Service>
    static auto getServiceType() noexcept -> ServiceType { return typeid(Service); }

protected:
    Services           m_services       {}; ///< Services.
    mutable std::mutex m_services_guard {}; ///< Services guard.
};

template<typename    ConcreteService,
         typename... Args>
    requires std::is_base_of_v<Service, ConcreteService>
inline auto ServiceManager::publish(Args&&... args) -> std::shared_ptr<ConcreteService>
{
    std::lock_guard lock { m_services_guard };

    const auto& service_type = getServiceType<ConcreteService>();
    if (m_services.contains(service_type))
        throw RuntimeException("Service has already been published");

    auto service = std::make_shared<ConcreteService>(std::forward<Args>(args)...);
    service->init();

    m_services.insert(std::make_pair(service_type, std::shared_ptr<ConcreteService>(service)));
    return service;
}

template<typename ConcreteService>
    requires std::is_base_of_v<Service, ConcreteService>
auto ServiceManager::getService() noexcept -> std::shared_ptr<ConcreteService>
{
    std::lock_guard lock { m_services_guard };

    const auto& service_type = getServiceType<ConcreteService>();
    if (!m_services.contains(service_type))
        return {};

    auto service =
        std::static_pointer_cast<ConcreteService>(m_services.at(service_type));

    return std::shared_ptr<ConcreteService>(service);
}

template<typename ConcreteService>
    requires std::is_base_of_v<Service, ConcreteService>
auto ServiceManager::terminate() -> bool
{
    std::lock_guard lock { m_services_guard };

    const auto& service_type = getServiceType<ConcreteService>();
    if (!m_services.contains(service_type))
        return false;

    auto& service = m_services.at(service_type);
    service->stop();
    m_services.erase(service_type);

    return true;
}

inline auto ServiceManager::terminate() -> void
{
    std::lock_guard lock { m_services_guard };

    for (auto& [_, service] : m_services)
    {
        try
        {
            service->stop();
        }
        catch (const std::exception& ex)
        {
            // g_logger->error("Cannot stop service: {}", ex.what());
        }
    }

    m_services.clear();
}

} // namespace isml

#endif // ISML_SERVICE_MANAGER_HPP
