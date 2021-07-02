/**
 * @file    task_executor.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    7/12/20
 */

#ifndef ISML_TASK_EXECUTOR_HPP
#define ISML_TASK_EXECUTOR_HPP

#include <type_traits>

#include <isml/task/task.hpp>

namespace isml {

/**
 * @class   TaskExecutor
 * @brief   Serves as a base class for types intended for tasks execution.
 */

class TaskExecutor
{
public:
    using SharedAction = std::shared_ptr<TaskBase::ActionBase>;

public:
    TaskExecutor() = default;
    virtual ~TaskExecutor() = default;

    // Non-copyable
    TaskExecutor(const TaskExecutor&) = delete;
    auto operator=(const TaskExecutor&) -> TaskExecutor& = delete;

    // Non-movable
    TaskExecutor(TaskExecutor&&) = delete;
    auto operator=(TaskExecutor&&) -> TaskExecutor& = delete;

public:
    template<typename Fn, typename... Args>
    auto execute(Fn&& fn, Args&&... args) noexcept -> decltype(auto);

    template<typename Fn, typename... Args>
    auto submit(Fn&& fn, Args&&... args) noexcept -> decltype(auto);

    auto terminate() -> void;
    virtual auto isTerminating() const noexcept -> bool;

protected:
    virtual auto doExecute(SharedAction&& action) -> void { action->perform(); };
    virtual auto doTerminate() -> void {}
    virtual auto doTerminatingCheck() const noexcept -> bool { return false; }
};

// Definitions

template<typename Fn, typename... Args>
auto TaskExecutor::execute(Fn&& fn, Args&&... args) noexcept -> decltype(auto)
{
    Task task { std::forward<Fn>(fn), std::forward<Args>(args)... };
    doExecute(task.action()->shared_from_this());
}

template<typename Fn, typename... Args>
auto TaskExecutor::submit(Fn&& fn, Args&&... args) noexcept -> decltype(auto)
{
    Task task { std::forward<Fn>(fn), std::forward<Args>(args)... };
    doExecute(task.action()->shared_from_this());
    return task;
}

inline auto TaskExecutor::terminate() -> void
{
    doTerminate();
}

inline auto TaskExecutor::isTerminating() const noexcept -> bool
{
    return doTerminatingCheck();
}

} // namespace isml

#endif // ISML_TASK_EXECUTOR_HPP
