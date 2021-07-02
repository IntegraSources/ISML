/**
 * @file    message_controller.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    8/24/20
 */

#include <isml/message/message_controller.hpp>

namespace isml {

MessageController::MessageController(SessionManager& session_manager)
    : m_session_manager(&session_manager)
{}

auto MessageController::init() -> void
{
    doInit();
}

auto MessageController::dispatcher() noexcept -> MessageDispatcher&
{
    return m_message_dispatcher;
}

auto MessageController::doInit() -> void
{
    setUpMessageChannels();
    setUpMessageDispatcher();
}

} // namespace isml
