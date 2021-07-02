/**
 * @file    session_factory.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    10/02/2020
 */

#ifndef ISML_SESSION_FACTORY_HPP
#define ISML_SESSION_FACTORY_HPP

#include <atomic>

#include <isml/session/session.hpp>

namespace isml {

/**
 * @class   SessionFactory
 * @class   A factory for creating messaging sessions.
 */

class SessionFactory
{
public:
    SessionFactory() = default;
    virtual ~SessionFactory() = default;

    // non-copyable
    SessionFactory(const SessionFactory&) = delete;
    auto operator=(const SessionFactory&) -> SessionFactory& = delete;

public:
    auto createSession(std::unique_ptr<Transport> transport) -> Session::Ptr;

protected:
    auto generateId() -> SessionId;

private:
    std::atomic<SessionId> m_session_id {}; ///< Current identifier.
};

} // namespace isml

#endif // ISML_SESSION_FACTORY_HPP
