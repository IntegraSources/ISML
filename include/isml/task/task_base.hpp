/**
 * @file	task_base.hpp
 * @author	Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date	7/11/20
 */

#ifndef ISML_TASK_BASE_HPP
#define ISML_TASK_BASE_HPP

/**
 * @class   TaskBase
 * @brief   Serves as a base class for the Task class template and other types
 *          representing asynchronous and/or lazily operation.
 */

class TaskBase
{
public:

    /**
     * @enum    Status
     * @brief   Specifies task execution statuses.
     */

    enum class Status
    {
        Created,        ///< Task just created.
        InProgress,     ///< The task is executing at the moment.
        Completed,      ///< Task execution completed.
        Cancelled       ///< Task execution was canceled.
    };

    /**
     * @enum    CancellationResult
     * @brief   Specifies task cancellation results.
     */

    enum class CancellationResult
    {
        Success,                ///< Task successfully canceled.
        TaskAlreadyInProgress,  ///< Task cannot be canceled because it already in progress.
        TaskAlreadyCompleted    ///< Task cannot be canceled because it already completed.
    };

    /**
     * @class   ActionBase
     * @brief   Serves as a base class for Action class template.
     */

    class ActionBase
    {
    public:
        virtual ~ActionBase() = default;

        // Non-copyable
        ActionBase(const ActionBase&) = delete;
        auto operator=(const ActionBase&) -> ActionBase& = delete;

        // Non-movable
        ActionBase(ActionBase&&) = delete;
        auto operator=(ActionBase&&) -> ActionBase& = delete;

    protected:
        ActionBase() = default;

    public:
        virtual auto perform() noexcept -> void = 0;
    };

public:
    virtual ~TaskBase() = default;

    // Non-copyable
    TaskBase(const TaskBase&) = delete;
    auto operator=(const TaskBase&) -> TaskBase& = delete;

protected:
    TaskBase() = default;

    // Movable
    TaskBase(TaskBase&&) = default;
    auto operator=(TaskBase&&) -> TaskBase& = default;

public:
    virtual auto isCancelled() const noexcept -> bool = 0;

    virtual auto isCompleted() const noexcept -> bool = 0;

    virtual auto tryCancel() noexcept -> CancellationResult = 0;
};

#endif // ISML_TASK_BASE_HPP
