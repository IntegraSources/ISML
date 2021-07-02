/**
 * @file    service_manager.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/30/20
 */

#include <isml/service/service_manager.hpp>

namespace isml {

auto ServiceManager::getInstance() noexcept -> ServiceManager&
{
    static ServiceManager instance;
    return instance;
}

} // namespace isml
