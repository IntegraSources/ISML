/**
 * @file    task_scheduler.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    7/13/20
 */

#ifndef ISML_TASK_SCHEDULER_HPP
#define ISML_TASK_SCHEDULER_HPP

#include <utility>
#include <memory>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <compare>

#include <isml/task/task_base.hpp>
#include <isml/task/task.hpp>

#include <isml/executors/thread_pool_task_executor.hpp>

using namespace std::chrono_literals;

namespace isml {

/**
 * @class   TaskScheduler
 * @tparam  Executor  The concrete task executor type.
 */

template<typename Executor = ThreadPoolTaskExecutor,
         typename Clock = std::chrono::high_resolution_clock>
class TaskScheduler
    : public Executor
{
protected:
    using SharedAction = typename Executor::SharedAction;
    using Duration = std::chrono::milliseconds;
    using Timestamp = typename Clock::time_point;

    enum class SuspendReason : unsigned
    {
        UpdateDelay = 0,
        ExecuteTask,
        Terminate
    };

    /**
     * @class   ScheduledAction
     * @brief   An action that contains additional information about start
     *          point and delay.
     */

    class ScheduledAction
    {
    public:
        ScheduledAction(SharedAction&& action, Duration delay);
        virtual ~ScheduledAction() = default;

        // Non-movable
        ScheduledAction(const ScheduledAction&) = delete;
        auto operator=(const ScheduledAction&) -> ScheduledAction& = delete;

        // Movable
        ScheduledAction(ScheduledAction&& other) noexcept;
        auto operator=(ScheduledAction&& other) noexcept -> ScheduledAction&;

    protected:
        ScheduledAction() = default;

    public:
        auto delay() const noexcept -> const Duration&;
        auto action() noexcept -> SharedAction&;
        auto timeLeft() const noexcept -> Duration;

        auto copy() const noexcept -> ScheduledAction;

        auto operator<=>(const ScheduledAction& rhs) const -> auto;

    protected:
        SharedAction m_action      {}; ///< Action to be performed.
        Duration     m_delay       {}; ///< Delay.
        Timestamp    m_creation_ts {}; ///< Creation time stamp.
    };

    using ScheduledActions =
        std::priority_queue<ScheduledAction
                          , std::vector<ScheduledAction>
                          , std::greater<>
                          >;

public:
    template<typename... Args>
    explicit TaskScheduler(Args&&... args);

    ~TaskScheduler() override;

public:
    template<typename Fn, typename... Args>
    auto schedule(Fn&& fn, Duration delay, Args&&... args) -> decltype(auto);

    static auto getInstance() -> TaskScheduler&;

protected:
    auto timeLeft() const noexcept -> Duration;
    auto doTerminate() -> void override;

    static auto maxWaitTime() -> Duration;

private:
    virtual auto doSchedule(SharedAction&& action, Duration delay) -> void;

    auto watcherFn() -> void;

protected:
    ScheduledActions           m_scheduled_actions       {}; ///< Scheduled actions.
    std::mutex                 m_scheduled_actions_guard {}; ///< Action list guard.
    std::condition_variable    m_cv                      {}; ///< Conditional variable.
    Duration                   m_delay;                      ///< Current delay.
    std::thread                m_watcher;                    ///< Watch thread.
    Timestamp                  m_delay_update_ts         = Clock::now();
    std::atomic<SuspendReason> m_suspend_reason          = SuspendReason::UpdateDelay;
};

// Definitions

template<typename Executor, typename Clock>
inline TaskScheduler<Executor, Clock>::ScheduledAction::ScheduledAction(SharedAction&& action, Duration delay)
    : m_action(std::move(action))
    , m_delay(delay)
    , m_creation_ts(Clock::now())
{}

template<typename Executor, typename Clock>
TaskScheduler<Executor, Clock>::~TaskScheduler()
{
    if (m_watcher.joinable())
    {
        m_watcher.join();
    }
    Executor::terminate();
}

template<typename Executor, typename Clock>
inline TaskScheduler<Executor, Clock>::ScheduledAction::ScheduledAction(ScheduledAction&& other) noexcept
    : m_action(std::exchange(other.m_action, {}))
    , m_delay(std::exchange(other.m_delay, {}))
    , m_creation_ts(std::exchange(other.m_creation_ts, {}))
{}

template<typename Executor, typename Clock>
inline auto TaskScheduler<Executor, Clock>::ScheduledAction::operator=(ScheduledAction&& other) noexcept -> ScheduledAction&
{
    if (this != std::addressof(other))
    {
        m_delay = std::exchange(other.m_delay, {});
        m_action = std::exchange(other.m_action, {});
        m_creation_ts = std::exchange(other.m_creation_ts, {});
    }

    return *this;
}

template<typename Executor, typename Clock>
inline auto TaskScheduler<Executor, Clock>::ScheduledAction::delay() const noexcept -> const Duration&
{
    return m_delay;
}

template<typename Executor, typename Clock>
inline auto TaskScheduler<Executor, Clock>::ScheduledAction::action() noexcept -> SharedAction&
{
    return m_action;
}

template<typename Executor, typename Clock>
inline auto TaskScheduler<Executor, Clock>::ScheduledAction::timeLeft() const noexcept -> Duration
{
    return m_delay - std::chrono::duration_cast<Duration>(Clock::now() - m_creation_ts);
}

template<typename Executor, typename Clock>
auto TaskScheduler<Executor, Clock>::ScheduledAction::copy() const noexcept -> TaskScheduler::ScheduledAction
{
    ScheduledAction action;
    action.m_delay = m_delay;
    action.m_action = std::move(m_action);
    action.m_creation_ts = std::move(m_creation_ts);
    return action;
}

template<typename Executor, typename Clock>
inline auto TaskScheduler<Executor, Clock>::ScheduledAction::operator<=>(
    const typename TaskScheduler<Executor, Clock>::ScheduledAction& rhs) const -> auto
{
    const auto tl1 = timeLeft();
    const auto tl2 = rhs.timeLeft();

    if (tl1 == tl2)
        return std::weak_ordering::equivalent;
    else if (tl1 < tl2)
        return std::weak_ordering::less;
    else
        return std::weak_ordering::greater;
}

template<typename Executor, typename Clock>
template<typename... Args>
inline TaskScheduler<Executor, Clock>::TaskScheduler(Args&& ... args)
    : Executor(std::forward<Args>(args)...)
    , m_delay(TaskScheduler::maxWaitTime())
    , m_watcher(std::bind(&TaskScheduler::watcherFn, this))
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(3s);
}

template<typename Executor, typename Clock>
template<typename Fn, typename... Args>
auto TaskScheduler<Executor, Clock>::schedule(Fn&& fn, Duration delay, Args&&... args) -> decltype(auto)
{
    Task task { std::forward<Fn>(fn), std::forward<Args>(args)... };
    doSchedule(task.action()->shared_from_this(), delay);
    return task;
}

template<typename Executor, typename Clock>
inline auto TaskScheduler<Executor, Clock>::timeLeft() const noexcept -> TaskScheduler::Duration
{
    return m_delay - std::chrono::duration_cast<Duration>(Clock::now() - m_delay_update_ts);
}

template<typename Executor, typename Clock>
auto TaskScheduler<Executor, Clock>::doTerminate() -> void
{
    std::lock_guard lock { m_scheduled_actions_guard };
    m_suspend_reason = SuspendReason::Terminate;
    m_cv.notify_one();
}

template<typename Executor, typename Clock>
auto TaskScheduler<Executor, Clock>::maxWaitTime() -> Duration
{
    return std::chrono::duration_cast<Duration>(
        std::chrono::time_point<Clock>::max() - Clock::now() - std::chrono::hours(1));
}

template<typename Executor, typename Clock>
inline auto TaskScheduler<Executor, Clock>::getInstance() -> TaskScheduler&
{
    static TaskScheduler<> scheduler;
    return scheduler;
}

template<typename Executor, typename Clock>
auto TaskScheduler<Executor, Clock>::doSchedule(SharedAction&& action, Duration delay) -> void
{
    using namespace std::chrono_literals;

    std::lock_guard lock { m_scheduled_actions_guard };

    if (this->isTerminating())
    {
        return;
    }

    ScheduledAction scheduled_action { std::move(action), delay };
    m_scheduled_actions.push(std::move(scheduled_action));

    if (timeLeft() > delay)
    {
        m_delay = delay;
        m_suspend_reason = SuspendReason::UpdateDelay;
        m_cv.notify_one();
    }
}

template<typename Executor, typename Clock>
auto TaskScheduler<Executor, Clock>::watcherFn() -> void
{
    using namespace std::chrono_literals;

    while (!this->isTerminating())
    {
        std::unique_lock lock { m_scheduled_actions_guard };
        m_cv.wait_for(lock, m_delay);

        switch (m_suspend_reason)
        {
            case SuspendReason::UpdateDelay:
                {
                    m_delay_update_ts = Clock::now();
                    m_suspend_reason = SuspendReason::ExecuteTask;
                }
                break;

            case SuspendReason::ExecuteTask:
                {
                    if (!m_scheduled_actions.empty())
                    {
                        auto scheduled_action = m_scheduled_actions.top().copy();
                        m_scheduled_actions.pop();
                        this->doExecute(std::move(scheduled_action.action()));
                    }

                    m_delay = (!m_scheduled_actions.empty())
                        ? m_scheduled_actions.top().timeLeft()
                        : maxWaitTime();

                    m_delay_update_ts = Clock::now();
                }
                break;

            case SuspendReason::Terminate:
                return;
        }
    }
}

} // namespace isml

#endif // ISML_TASK_SCHEDULER_HPP
