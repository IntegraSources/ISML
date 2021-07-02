/**
 * @file    deferred_action.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    10/26/20
 */

#ifndef ISML_DEFERRED_CALL_HPP
#define ISML_DEFERRED_CALL_HPP

#include <functional>
#include <type_traits>

#define ISML_DEFER(ACTION, ...) \
const auto ISML_CONCAT(action__, __LINE__) = isml::defer(ACTION, __VA_ARGS__);

#define ISML_DEFER_NO_ARGS(ACTION) \
const auto ISML_CONCAT(action__, __LINE__) = isml::defer(ACTION);

namespace isml {

/**
 * @class   DeferredCall
 * @brief   A wrapper for a callable object that defers its invocation to the
 *          end of its lifetime.
 */

class DeferredCall
{
public:
    using WrappedCall = std::function<void()>;

public:
    DeferredCall() = default;
    explicit DeferredCall(WrappedCall call);
    virtual ~DeferredCall() noexcept;

    // Non-copyable
    DeferredCall(const DeferredCall&) = delete;
    auto operator=(const DeferredCall&) -> DeferredCall& = delete;

    // Movable
    DeferredCall(DeferredCall&&) = default;
    auto operator=(DeferredCall&&) -> DeferredCall& = default;

protected:
    WrappedCall m_call {};
};

inline DeferredCall::~DeferredCall() noexcept
{
    try
    {
        if (m_call) m_call();
    }
    catch (...)
    {}
}

inline DeferredCall::DeferredCall(WrappedCall call)
    : m_call(std::move(call))
{}

/**
 * @brief   Creates deferrable object from the specified callable object.
 *
 * @tparam  F     Callable object type.
 * @tparam  Ts    Arguments types.
 *
 * @param   fn    Callable object.
 * @param   args  Arguments to pass to f.
 *
 * @return  An object of the DeferredCall type.
 */

template<typename F, typename... Args>
inline auto defer(F&& fn, Args&&... args) -> decltype(auto)
{
    return DeferredCall {
        std::bind(std::forward<F>(fn), std::forward<Args>(args)...)
    };
}

/**
 * @brief   Defers call to a pointer to a function.
 *
 * @tparam  R     Return type.
 * @tparam  Args  Arguments types.
 *
 * @param   f     Pointer to function.
 * @param   args  Function arguments.
 *
 * @return  An object of the DeferredCall type.
 */

template<typename R, typename... Args>
inline auto defer(R(*fn)(Args...), Args&&... args) -> decltype(auto)
{
    return DeferredCall {
        std::bind(fn, std::forward<Args>(args)...)
    };
}

/**
 * @brief   Defers call to a member-function.
 *
 * @tparam  R     Return type.
 * @tparam  C     Class.
 * @tparam  Args  Arguments types.
 *
 * @param   fn    Member-function.
 * @param   obj   Instance.
 * @param   args  Member-function arguments.
 *
 * @return  An object of the DeferredCall type.
 */

template<typename R, typename C, typename... Args>
inline auto defer(R(C::*fn)(Args...), C& obj, Args&&... args) -> decltype(auto)
{
    return DeferredCall {
        std::bind(fn, std::ref(obj), std::forward<Args>(args)...)
    };
}

} // namespace isml

#endif // ISML_DEFERRED_CALL_HPP
