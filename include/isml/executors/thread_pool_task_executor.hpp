/**
 * @file	thread_pool_task_executor.hpp
 * @author	Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date	7/13/20
 */

#ifndef ISML_THREAD_POOL_TASK_EXECUTOR_HPP
#define ISML_THREAD_POOL_TASK_EXECUTOR_HPP

#include <mutex>
#include <thread>
#include <atomic>

#include <queue>
#include <vector>

#include <isml/task/task_executor.hpp>

namespace isml {

/**
 * @class   ThreadPoolTaskExecutor
 * @brief   Provides a pool of threads that can be used to execute tasks.
 */

class ThreadPoolTaskExecutor
    : public TaskExecutor
{
public:
    explicit ThreadPoolTaskExecutor(std::size_t worker_count = std::thread::hardware_concurrency());
    ~ThreadPoolTaskExecutor() override;

    // Non-copyable
    ThreadPoolTaskExecutor(const ThreadPoolTaskExecutor&) = delete;
    auto operator=(const ThreadPoolTaskExecutor&) -> ThreadPoolTaskExecutor& = delete;

    // Non-movable
    ThreadPoolTaskExecutor(ThreadPoolTaskExecutor&&) = delete;
    auto operator=(ThreadPoolTaskExecutor&&) -> ThreadPoolTaskExecutor& = delete;

protected:
    auto startWorker() -> void;

protected:
    auto doExecute(SharedAction&& action) -> void override;
    auto doTerminate() -> void override;
    auto doTerminatingCheck() const noexcept -> bool override;

protected:
    std::atomic_bool         m_done          {}; ///< Indicates that thread pool will be terminated.
    std::vector<std::thread> m_workers       {}; ///< Workers.
    std::queue<SharedAction> m_actions       {}; ///< Action to be performed.
    std::mutex               m_actions_guard {}; ///< Action queue guard.
    std::condition_variable  m_cv            {}; ///< Conditional variable.
};

// Definitions

inline auto ThreadPoolTaskExecutor::doTerminate() -> void
{ m_done = true; }

inline auto ThreadPoolTaskExecutor::doTerminatingCheck() const noexcept -> bool
{ return m_done; }

} // namespace isml

#endif // ISML_THREAD_POOL_TASK_EXECUTOR_HPP
