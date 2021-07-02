/**
 * @file    message_controller.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    8/24/20
 */

#ifndef ISML_MESSAGE_CONTROLLER_HPP
#define ISML_MESSAGE_CONTROLLER_HPP

#include <isml/message/message_dispatcher.hpp>

#include <isml/session/session_manager.hpp>

namespace isml {

/**
 * @class   MessageController
 * @brief   Serves as a base class for all message controllers.
 */

class MessageController
{
public:
    explicit MessageController(SessionManager& session_manager);
    virtual ~MessageController() = default;

    MessageController(const MessageController&) = delete;
    auto operator=(const MessageController&) -> MessageController& = delete;

public:
    auto init() -> void;

    auto dispatcher() noexcept -> MessageDispatcher&;

private:
    virtual auto doInit() -> void;

    virtual auto setUpMessageDispatcher() -> void = 0;
    virtual auto setUpMessageChannels() -> void = 0;

protected:
    MessageDispatcher m_message_dispatcher {}; ///< Message dispatcher.
    SessionManager*   m_session_manager;       ///< Session manager.
};

} // namespace isml

#endif // ISML_MESSAGE_CONTROLLER_HPP
