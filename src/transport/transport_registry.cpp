/**
 * @file    transport_registry.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    06.03.2021
 */

#include <isml/transport/transport_registry.hpp>

#include <utility>

namespace isml {

auto TransportRegistry::registerFactory(std::unique_ptr<TransportFactory> factory) -> void
{
    m_factories.push_back(std::move(factory));
}

auto TransportRegistry::getFactory(const std::string& protocol) const noexcept -> Maybe<TransportFactory&>
{
    Maybe<TransportFactory&> maybe { none };
    for (const auto& factory : m_factories)
    {
        if (factory->supports(protocol))
        {
            maybe = *factory;
        }
    }
    return maybe;
}

} // namespace isml
