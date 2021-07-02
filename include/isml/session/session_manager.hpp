/**
 * @file    session_manager.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    10/02/2020
 */

#ifndef ISML_SESSION_MANAGER_HPP
#define ISML_SESSION_MANAGER_HPP

#include <unordered_map>
#include <mutex>
#include <functional>
#include <optional>

#include <isml/session/session.hpp>
#include <isml/session/session_factory.hpp>

namespace isml {

/**
 * @class  SessionManager
 * @brief  Manages messaging sessions.
 */

class SessionManager
{
public:
    using Sessions = std::unordered_map<SessionId, std::shared_ptr<Session>>;       ///< Session storage type.
    using SessionPredicate = std::function<bool(const std::shared_ptr<Session>&)>;  ///< Session predicate type.
    using SessionHandler = std::function<void(Session::Ptr&)>;                      ///< Session handler type.

public:
    SessionManager();
    virtual ~SessionManager() = default;

    // non-copyable
    SessionManager(const SessionManager&) = delete;
    auto operator=(const SessionManager&) -> SessionManager& = delete;

public:

    /**
     * @brief   Creates a new session.
     *
     * @return  A pointer to the created session instance.
     */

    auto createSession(std::unique_ptr<Transport> transport) -> std::shared_ptr<Session>;

    /**
     * @brief   Checks if a session exists in the system.
     *
     * @param   id  A session identifier.
     *
     * @return  If a session exists - returns true, otherwise - false.
     */

    auto hasSession(SessionId id) const noexcept -> bool;

    /**
     * Terminates a session by the specified identifier.
     * @param  id  A session identifier.
     */

    auto terminate(SessionId id) -> void;

    auto terminateAll() -> void;

    auto forEach(const std::function<void(Session::Ptr&)>& session) -> void;

    auto find(SessionPredicate predicate) -> std::shared_ptr<Session>;

    template<typename T, typename Key = Session::Properties::Key>
    auto findSessionByProperty(const Key& key, const T& val) -> Session::Ptr;

protected:
    auto terminateSessionNoLock(SessionId id) -> void;

public:
    SessionHandler onSessionTerminating {}; ///< Will be invoked before the session termination.
    SessionHandler onSessionTerminated  {}; ///< Will be invoked after the session termination.
    SessionHandler onSessionOpened      {}; ///< Will be invoked after the session open.

protected:
    SessionFactory     m_session_factory;  ///< A session factory.
    Sessions           m_sessions;         ///< Messaging sessions.
    mutable std::mutex m_sessions_guard;   ///< Used on operations with sessions.
};

template<typename T, typename Key>
auto SessionManager::findSessionByProperty(const Key& key, const T& val) -> Session::Ptr
{
    return find([&](const Session::Ptr& session)
        {
            return session->properties[key] == val;
        });
}

} // namespace isml

#endif // ISML_SESSION_MANAGER_HPP
