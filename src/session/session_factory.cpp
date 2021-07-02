/**
 * @file    session_factory.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    10/02/2020
 */

#include <isml/session/session_factory.hpp>

namespace isml {

auto SessionFactory::generateId() -> SessionId
{
    return ++m_session_id;
}

auto SessionFactory::createSession(std::unique_ptr<Transport> transport) -> Session::Ptr
{
    return Session::createNew(generateId(), std::move(transport));
}

} // namespace isml
