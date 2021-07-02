/**
 * @file    transport_registry.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    03.03.2021
 */

#ifndef ISML_TRANSPORT_REGISTRY_HPP
#define ISML_TRANSPORT_REGISTRY_HPP

#include <memory>
#include <list>

#include <isml/base/type_traits.hpp>
#include <isml/base/maybe.hpp>
#include <isml/transport/transport_factory.hpp>

namespace isml {

class TransportRegistry
{
public:
    using Factories = std::list<std::unique_ptr<TransportFactory>>;

public:
    TransportRegistry() = default;

public:
    template<typename ConcreteFactory, typename... Args>
        requires std::is_base_of_v<TransportFactory, ConcreteFactory>
    auto registerFactory(Args&&... args) -> void;

    auto registerFactory(std::unique_ptr<TransportFactory> factory) -> void;

    auto getFactory(const std::string& protocol) const noexcept -> Maybe<TransportFactory&>;

protected:
    Factories m_factories {};
};

template<typename ConcreteFactory, typename... Args>
    requires std::is_base_of_v<TransportFactory, ConcreteFactory>
auto TransportRegistry::registerFactory(Args&&... args) -> void
{
    registerFactory(std::make_unique<ConcreteFactory>(std::forward<Args>(args)...));
}

} // namespace isml

#endif // ISML_TRANSPORT_REGISTRY_HPP
