/**
 * @file    session.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    10/02/2020
 */

#ifndef ISML_SESSION_HPP
#define ISML_SESSION_HPP

#include <memory>
#include <optional>

#include <isml/base_types.hpp>

#include <isml/message/message.hpp>

#include <isml/transport/transport.hpp>

#include <isml/session/session_base.hpp>

namespace isml {

/**
 * @class   Session
 * @brief   Represents messaging session.
 */

ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic push)
ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wnon-virtual-dtor")

class Session
    : public SessionBase
    , public std::enable_shared_from_this<Session>
{
public:
    using Ptr = std::shared_ptr<Session>;

public:
    Session() = delete;
    Session(SessionId id, std::unique_ptr<Transport> transport);

    // non-copyable
    Session(const Session&) = delete;
    auto operator=(const Session&) -> Session = delete;

    // non-movable
    Session(Session&&) = delete;
    auto operator=(Session&&) -> Session& = delete;

public:
    auto id() const noexcept -> SessionId;

    auto send(Message::Ptr msg) -> void;
    auto receive() -> std::optional<Message::Ptr>;
    auto request(Message::Ptr msg) -> FutureMessage;

    auto shutdown() -> void;

    auto active() const noexcept -> bool;

    static auto createNew(SessionId session_id, std::unique_ptr<Transport> transport) -> Session::Ptr;

    auto transport() noexcept -> std::unique_ptr<Transport>&;

public:
    Properties properties {};

private:
    const SessionId            m_id;         ///< Session identifier.
    std::unique_ptr<Transport> m_transport;  ///< Message transport.

    friend class SessionFactory;
};

ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic pop)

} // namespace isml

#endif // ISML_SESSION_HPP
