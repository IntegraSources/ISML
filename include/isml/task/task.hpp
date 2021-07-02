/**
 * @file    task.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    7/11/20
 */

#ifndef ISML_TASK_HPP
#define ISML_TASK_HPP

#include <memory>
#include <atomic>
#include <future>
#include <thread>
#include <functional>
#include <utility>
#include <cassert>

#include <isml/exceptions.hpp>
#include <isml/task/task_base.hpp>

namespace isml {

/**
 * @class   Task
 * @brief   A task represents an operation that produces result.
 *
 * @tparam  T  The result type.
 */

template<typename T>
class Task
    : public TaskBase
{
public:

    /**
     * @class   Mode
     * @brief   A task execution state stores the current execution status and
     *          allows to control execution process.
     */

    class State final
        : public std::enable_shared_from_this<State>
    {
    public:

        // Non-copyable
        State(const State&) = delete;
        auto operator=(const State&) -> State& = delete;

        // Non-movable
        State(State&&) = delete;
        auto operator=(State&&) -> State& = delete;

    private:
        State() = default;

    public:
        template<typename... Ts>
        static auto create(Ts&&... args) noexcept -> std::shared_ptr<State>;

        auto isCompleted() const noexcept -> bool;

        auto isCancelled() const noexcept -> bool;

        auto tryCancel() noexcept -> CancellationResult;

        auto trySetInProgress() noexcept -> bool;

        auto trySetCompleted() noexcept -> bool;

    private:
        std::atomic<Status> m_status = Status::Created; ///< Task execution status.
    };

    using SharedState = std::shared_ptr<State>; ///< The shared state type.

    /**
     * @class   Action
     * @brief   An action is a container for operation.
     *
     * @tparam  U  The operation result type.
     */

ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic push)
ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wnon-virtual-dtor")

    template<typename U>
    class Action final
        : public ActionBase
        , public std::enable_shared_from_this<Action<U>>
    {
    public:

        // Non-copyable
        Action(const Action&) = delete;
        auto operator=(const Action&) -> Action& = delete;

        // Non-movable
        Action(Action&&) = delete;
        auto operator=(Action&&) -> Action& = delete;

    private:
        template<typename Fn, typename... Args>
            requires std::is_convertible_v<std::invoke_result_t<Fn, Args...>, U>
        explicit Action(SharedState state, Fn&& fn, Args&&... args) noexcept;

    public:
        template<typename Fn, typename... Args>
            requires std::is_convertible_v<std::invoke_result_t<Fn, Args...>, U>
        static auto create(SharedState state, Fn&& fn, Args&&... args) noexcept -> decltype(auto);

        auto perform() noexcept -> void override;

        auto awaiter() -> std::future<U>&;

    private:
        SharedState         m_state   {}; ///< Task state.
        std::function<U()>  m_fn      {}; ///< An operation to be performed.
        std::promise<U>     m_promise {}; ///< Promise object.
        std::future<U>      m_awaiter {}; ///< Awaiter object.
    };

    using SharedAction = std::shared_ptr<Action<T>>; ///< The shared action type.

public:
    Task() = delete;

    template<typename Fn, typename... Args>
    explicit Task(Fn&& fn, Args&&... args) noexcept;

    // Non-copyable
    Task(const Task&) = delete;
    auto operator=(const Task&) -> Task& = delete;

    // Non-move constructible
    Task(Task&&) noexcept;
    auto operator=(Task&& other) noexcept -> Task&;

public:
    auto run() noexcept -> void;

    auto runAsync() noexcept -> void;

    auto getResult() -> T;

    auto isCancelled() const noexcept -> bool override;

    auto isCompleted() const noexcept -> bool override;

    auto tryCancel() noexcept -> CancellationResult override;

    auto action() noexcept -> SharedAction&;

protected:
    SharedState     m_shared_state  {}; ///< Shared state.
    SharedAction    m_shared_action {}; ///< Shared action.
};

ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic pop)

template<typename Fn, typename... Args>
Task(Fn&& fn, Args&&... args) -> Task<std::invoke_result_t<Fn, Args...>>;

// Definitions

template<typename T>
template<typename... Ts>
inline auto Task<T>::State::create(Ts&&... args) noexcept -> std::shared_ptr<State>
{
    return SharedState { new State { std::forward<Ts>(args)... } };
}

template<typename T>
inline auto Task<T>::State::isCompleted() const noexcept -> bool
{
    return m_status == Status::Completed;
}

template<typename T>
inline auto Task<T>::State::isCancelled() const noexcept -> bool
{
    return m_status == Status::Cancelled;
}

template<typename T>
auto Task<T>::State::tryCancel() noexcept -> TaskBase::CancellationResult
{
    auto res = CancellationResult::Success;

    auto allowable_status = Status::Created;
    if (!m_status.compare_exchange_weak(allowable_status, Status::Cancelled))
    {
        if (allowable_status == Status::InProgress)
            res = CancellationResult::TaskAlreadyInProgress;
        else if (allowable_status == Status::Completed)
            res = CancellationResult::TaskAlreadyCompleted;
    }

    return res;
}

template<typename T>
auto Task<T>::State::trySetInProgress() noexcept -> bool
{
    auto allowable_status = Status::Created;
    return m_status.compare_exchange_weak(allowable_status, Status::InProgress);
}

template<typename T>
auto Task<T>::State::trySetCompleted() noexcept -> bool
{
    auto allowable_status = Status::InProgress;
    return m_status.compare_exchange_weak(allowable_status, Status::Completed);
}

template<typename T>
template<typename U>
template<typename Fn, typename... Args>
    requires std::is_convertible_v<std::invoke_result_t<Fn, Args...>, U>
inline Task<T>::Action<U>::Action(SharedState state, Fn&& fn, Args&& ... args) noexcept
{
    m_state = std::move(state);

    if constexpr(!sizeof...(args))
    {
        m_fn = fn;
    }
    else
    {
        m_fn = [fn = std::forward<Fn>(fn), ...args = std::forward<Args>(args)]() mutable
            {
                return std::invoke(std::forward<Fn>(fn), std::forward<Args>(args)...);
            };
    }

    m_awaiter = m_promise.get_future();
}

template<typename T>
template<typename U>
template<typename Fn, typename... Args>
    requires std::is_convertible_v<std::invoke_result_t<Fn, Args...>, U>
inline auto Task<T>::Action<U>::create(SharedState state, Fn&& fn, Args&& ... args) noexcept -> decltype(auto)
{
    return SharedAction { new Action<T> { std::move(state), std::forward<Fn>(fn), std::forward<Args>(args)... } };
}

template<typename T>
template<typename U>
auto Task<T>::Action<U>::perform() noexcept -> void
{
    if (m_state->isCancelled())
    {
        try
        {
            throw InvalidOperationException("Attempt to access the result of the canceled task");
        }
        catch (...)
        {
            m_promise.set_exception(std::current_exception());
        }

        return;
    }

    if (m_state->trySetInProgress())
    {
        try
        {
            if constexpr (std::is_void_v<T>)
            {
                m_fn();
                m_promise.set_value();
            }
            else
            {
                auto res = m_fn();
                m_promise.set_value(std::move(res));
            }
        }
        catch (...)
        {
            m_promise.set_exception(std::current_exception());
        }

        assert(m_state->trySetCompleted());
    }
}

template<typename T>
template<typename U>
inline auto Task<T>::Action<U>::awaiter() -> std::future<U>&
{
    return m_awaiter;
}

template<typename T>
template<typename Fn, typename... Args>
inline Task<T>::Task(Fn&& fn, Args&& ... args) noexcept
{
    m_shared_state = State::create();
    m_shared_action =
        Action<T>::create(m_shared_state->shared_from_this()
            , std::forward<Fn>(fn)
            , std::forward<Args>(args)...);
}

template<typename T>
inline Task<T>::Task(Task&& other) noexcept
{
    m_shared_state = std::exchange(other.m_shared_state, {});
    m_shared_action = std::exchange(other.m_shared_action, {});
}

template<typename T>
inline auto Task<T>::operator=(Task&& other) noexcept -> Task&
{
    if (this != std::addressof(other))
    {
        m_shared_state = std::exchange(other.m_shared_state, {});
        m_shared_action = std::exchange(other.m_shared_action, {});
    }

    return *this;
}

template<typename T>
inline auto Task<T>::run() noexcept -> void
{
    m_shared_action->perform();
}

template<typename T>
inline auto Task<T>::runAsync() noexcept -> void
{
    static auto execute_task =
        [](SharedAction&& action)
            {
                action->perform();
            };

    std::thread { execute_task, m_shared_action->shared_from_this() }.detach();
}

template<typename T>
auto Task<T>::getResult() -> T
{
    return m_shared_action->awaiter().get();
}

template<typename T>
inline auto Task<T>::isCancelled() const noexcept -> bool
{
    return m_shared_state->isCancelled();
}

template<typename T>
inline auto Task<T>::isCompleted() const noexcept -> bool
{
    return m_shared_state->isCompleted();
}

template<typename T>
inline auto Task<T>::tryCancel() noexcept -> CancellationResult
{
    return m_shared_state->tryCancel();
}

template<typename T>
auto Task<T>::action() noexcept -> Task::SharedAction&
{
    return m_shared_action;
}

} // namespace isml

#endif // ISML_TASK_HPP
