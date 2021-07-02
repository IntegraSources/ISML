/**
 * @file    listenable.tests.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    6/27/21
 */

ISML_DISABLE_WARNINGS_PUSH
#   include <gtest/gtest.h>
ISML_DISABLE_WARNINGS_POP

#include <isml/base/listenable.hpp>

class SessionListener
{
public:
    virtual ~SessionListener() = default;

    virtual auto onSessionStarted() -> void = 0;
    virtual auto onSessionFailure(int error_code) -> void = 0;
};

class Session :
    public isml::Listenable<SessionListener>
{
public:
    auto start() -> void
    {
        invoke(&SessionListener::onSessionStarted);
    }

    auto causeError(int error_code) -> void
    {
        invoke(&SessionListener::onSessionFailure, error_code);
    }
};

class SessionHealthMonitor : public SessionListener
{
public:
    auto onSessionStarted() -> void override {};
    auto onSessionFailure(int ec) -> void override { error_code = ec; };

public:
    int error_code {};
};

class SessionLogger : public SessionListener
{
public:
    auto onSessionStarted() -> void override { ++log_entries; };
    auto onSessionFailure(int) -> void override { ++log_entries; };

public:
    std::size_t log_entries {};
};

TEST(ListenableTests, InvokeAction)
{
    Session session;
    auto monitor = session.addListener<SessionHealthMonitor>();
    auto logger = session.addListener<SessionLogger>();

    session.start();
    session.causeError(0x01);

    ASSERT_EQ(monitor->error_code, 0x01);
    ASSERT_EQ(logger->log_entries, 2U);
}

TEST(ListenableTests, InvokeActionOnce)
{
    Session session;
    auto monitor = session.addListener<SessionHealthMonitor>();
    auto logger = session.addListener<SessionLogger>();

    session.invokeOnce(&SessionListener::onSessionFailure, 0x01);

    ASSERT_EQ(monitor->error_code, 0x01);
    ASSERT_EQ(logger->log_entries, 0U);
}

TEST(ListenableTests, AddListener)
{
    {
        Session session;
        auto listener = session.addListener<SessionHealthMonitor>();
        ASSERT_TRUE(session.hasListeners());
        ASSERT_TRUE(session.hasListener(listener));
    }

    {
        Session session;
        auto listener = std::make_shared<SessionHealthMonitor>();
        session += listener;
        ASSERT_TRUE(session.hasListeners());
        ASSERT_TRUE(session.hasListener(listener));
    }
}

TEST(ListenableTests, RemoveListener)
{
    {
        Session session;
        auto listener = session.addListener<SessionHealthMonitor>();
        ASSERT_TRUE(session.removeListener(listener));
        ASSERT_FALSE(session.hasListener(listener));
    }

    {
        Session session;
        auto listener = std::make_shared<SessionHealthMonitor>();
        session += listener;
        session -= listener;
        ASSERT_FALSE(session.hasListener(listener));
    }
}
