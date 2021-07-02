/**
 * @file    guard.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/16/20
 */

#ifndef ISML_GUARD_HPP
#define ISML_GUARD_HPP

#include <type_traits>
#include <utility>
#include <mutex>
#include <functional>

namespace isml {

/**
 * @class   Guard
 * @brief   A wrapper preventing API-races for an object which is can be accessed
 *          by multiple threads.
 *
 * @tparam  T      The type of object.
 * @tparam  Mutex  The type of mutex.
 */

template<typename T,
         typename Mutex = std::mutex>
class Guard final
{
public:
    using Object = std::remove_cvref_t<T>;

public:
    Guard() requires std::is_default_constructible_v<Object> = default;

    template<typename... Args>
    explicit Guard(Args&&... args);

    Guard(const Guard&) = delete;
    auto operator=(const Guard&) -> Guard& = delete;

    Guard(Guard&&) = delete;
    auto operator=(Guard&&) -> Guard& = delete;

    template<typename U>
        requires (!std::is_same_v<Guard<T>, U>)
    auto operator=(U&& object) -> Guard&;

public:

    /**
     * @brief   Applies the specified action to the stored object.
     *
     * @tparam  F       The type of action.
     * @param   action  The action.
     *
     * @return  If action produce a value - the type of a value,
     *          otherwise - void.
     */

    template<typename F>
        requires std::is_invocable_v<F, Object&>
    auto perform(F&& action) -> decltype(auto);

private:
    Object  m_object       {}; ///< Object.
    Mutex   m_object_guard {}; ///< Object guard.
};

template<typename T, typename Mutex>
template<typename... Args>
inline Guard<T, Mutex>::Guard(Args&&... args)
    : m_object(std::forward(args)...)
{}

template<typename T, typename Mutex>
template<typename U>
    requires (!std::is_same_v<Guard<T>, U>)
auto Guard<T, Mutex>::operator=(U&& object) -> Guard&
{
    std::lock_guard lock { m_object_guard };
    m_object = std::forward<U>(object);
    return *this;
}

template<typename T, typename Mutex>
template<typename F>
    requires std::is_invocable_v<F, std::remove_cvref_t<T>&>
auto Guard<T, Mutex>::perform(F&& action) -> decltype(auto)
{
    std::lock_guard lock { m_object_guard };
    return std::invoke(std::forward<F>(action), std::ref(m_object));
}

} // namespace isml

#endif // ISML_GUARD_HPP
