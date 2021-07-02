/**
 * @file    thread_pool_task_executor.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    7/13/20
 */

#include <isml/executors/thread_pool_task_executor.hpp>

namespace isml {

ThreadPoolTaskExecutor::ThreadPoolTaskExecutor(std::size_t worker_count)
{
    m_workers.reserve(worker_count);
    for (auto i = 0U; i < worker_count; ++i)
    {
        m_workers.emplace_back(std::bind(&ThreadPoolTaskExecutor::startWorker, this));
    }
}

ThreadPoolTaskExecutor::~ThreadPoolTaskExecutor()
{
    {
        std::lock_guard lock { m_actions_guard };
        m_done = true;
        m_cv.notify_all();
    }

    for (auto& worker : m_workers)
    {
        try
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
        catch (...)
        {}
    }
}

auto ThreadPoolTaskExecutor::startWorker() -> void
{
    while (!m_done)
    {
        {
            std::unique_lock lock { m_actions_guard };
            m_cv.wait(lock, [this]
            {
                return !m_actions.empty() or m_done;
            });
        }

        if (m_done) return;

        while (true)
        {
            if (m_actions_guard.try_lock())
            {
                if (m_actions.empty())
                {
                    m_actions_guard.unlock();
                    break;
                }
                auto action = std::move(m_actions.front());
                m_actions.pop();
                m_actions_guard.unlock();
                action->perform();
            }
        }
    }
}

auto ThreadPoolTaskExecutor::doExecute(SharedAction&& action) -> void
{
    std::lock_guard lock { m_actions_guard };
    m_actions.push(std::move(action));
    m_cv.notify_one();
}

} // namespace isml
