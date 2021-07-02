/**
 * @file    session_manager.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    10/02/2020
 */

#include <isml/session/session_manager.hpp>

#include <vector>

#include <isml/exceptions.hpp>

namespace isml {

static auto handler_stub = [](Session::Ptr&){};

SessionManager::SessionManager()
    : m_session_factory()
    , m_sessions()
    , m_sessions_guard()
{
    onSessionTerminating = handler_stub;
    onSessionTerminated = handler_stub;
}

auto SessionManager::createSession(std::unique_ptr<Transport> transport) -> std::shared_ptr<Session>
{
    std::lock_guard lock { m_sessions_guard };

    auto session = m_session_factory.createSession(std::move(transport));
    const auto session_id = session->id();

    const auto& [it, success] =
        m_sessions.insert(std::make_pair(session_id, std::move(session)));

    // session with such id already exists
    if (!success)
    {
        // throw DuplicateSessionIdException("Session with such ID has already registered", session_id);
    }

    if (onSessionOpened)
        onSessionOpened(it->second);

    return it->second->shared_from_this();
}

auto SessionManager::hasSession(SessionId id) const noexcept -> bool
{
    std::lock_guard lock { m_sessions_guard };
    return m_sessions.contains(id);
}

auto SessionManager::terminate(SessionId id) -> void
{
    std::lock_guard lock { m_sessions_guard };
    terminateSessionNoLock(id);
}

auto SessionManager::terminateAll() -> void
{
    std::lock_guard lock { m_sessions_guard };

    std::vector<SessionId> ids;
    for (const auto& [_, session] : m_sessions)
        ids.push_back(session->id());

    for (auto id : ids) terminateSessionNoLock(id);
}

auto SessionManager::terminateSessionNoLock(SessionId id) -> void
{
    if (auto it = m_sessions.find(id); it != m_sessions.end())
    {
        auto session = std::move(it->second);

        try
        {
            if (onSessionTerminating) onSessionTerminating(session);
            session->shutdown();
            m_sessions.erase(it);
            if (onSessionTerminated) onSessionTerminated(session);
        }
        catch (const std::exception& ex)
        {
            // g_logger->error("An error occurred during session termination: {}", ex.what());
        }
    }
}

auto SessionManager::forEach(const std::function<void(Session::Ptr&)>& action) -> void
{
    std::lock_guard lock { m_sessions_guard };

    try
    {
        for (auto& [session_id, session] : m_sessions)
        {
            action(session);
        }
    }
    catch (const std::exception& ex)
    {
        // g_logger->error("An error occurred during performing action: {}", ex.what());
    }
}

auto SessionManager::find(SessionPredicate predicate) -> std::shared_ptr<Session>
{
    std::lock_guard lock { m_sessions_guard };

    const auto item_it =
        std::find_if(m_sessions.cbegin(), m_sessions.cend(),
            [&](const auto& item){ return predicate(item.second); });

    return item_it != m_sessions.cend()
         ? item_it->second->shared_from_this()
         : nullptr;
}

} // namespace isml
