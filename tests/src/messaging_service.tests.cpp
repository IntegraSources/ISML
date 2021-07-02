/**
 * @file    messaging_service.tests.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    08.03.2021
 */

#include <chrono>

ISML_DISABLE_WARNINGS_PUSH
#   include <gtest/gtest.h>
ISML_DISABLE_WARNINGS_POP

#include <isml/messaging_service.hpp>

using namespace isml;
using namespace std::chrono_literals;

TEST(MessagingServiceTests, RunService)
{
    MessagingService service;
    service.start();
    ASSERT_TRUE(service.state() == Service::State::Started
             or service.state() == Service::State::StartPending);

    std::this_thread::sleep_for(1s);
    service.stop();
    ASSERT_TRUE(service.stopped());
}
