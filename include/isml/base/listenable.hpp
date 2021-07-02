/**
 * @file    listenable.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    6/27/21
 */

#ifndef ISML_LISTENABLE_HPP
#define ISML_LISTENABLE_HPP

#include <type_traits>
#include <memory>
#include <utility>
#include <vector>
#include <algorithm>

#include <isml/exceptions.hpp>

namespace isml {

/**
 * @class   Listenable
 * @brief   Serves as a base class for other classes that use listeners.
 * @tparam  Listener  The base type of listeners.
 * @since   0.1.5
 */

template<typename Listener>
class Listenable
{
protected:
    using Listeners = std::vector<std::shared_ptr<Listener>>;

public:
    Listenable() = default;

protected:
    ~Listenable() = default;

public:

    /**
     * @brief   Checks if there are listeners.
     *
     * @return  If there are added listeners - true, otherwise - false.
     */

    auto hasListeners() const noexcept -> bool;

    /**
     * @brief   Checks if the given listener has been added.
     *
     * @tparam  ConcreteListener  Listener type.
     * @param   listener          Listener.
     *
     * @return  If listener has been added - true, otherwise - false.
     */

    template<typename ConcreteListener>
        requires std::is_base_of_v<Listener, ConcreteListener>
    auto hasListener(std::shared_ptr<ConcreteListener>& listener) const noexcept -> bool;

    /**
     * @brief   Add the given listener.
     *
     * @tparam  ConcreteListener  Listener type.
     * @param   listener          Listener.
     */

    template<typename ConcreteListener>
        requires std::is_base_of_v<Listener, ConcreteListener>
    auto addListener(std::shared_ptr<ConcreteListener> listener) -> void;

    /**
     * @brief   Add the given listener.
     *
     * @tparam  ConcreteListener  Listener type.
     * @tparam  ListenerMutator   Listener mutator type.
     * @param   listener          Listener.
     * @param   mutate            Listener mutator.
     */

    template<typename ConcreteListener, typename ListenerMutator>
        requires std::is_base_of_v<Listener, ConcreteListener>
              && std::is_invocable_v<ListenerMutator, ConcreteListener&>
    auto addListener(std::shared_ptr<ConcreteListener> listener, ListenerMutator&& mutate) -> void;

    /**
     * @brief   Add listener of the specified type.
     *
     * @tparam  ConcreteListener  Listener type.
     * @tparam  Args              Argument types.
     * @param   args              Arguments.
     */

    template<typename ConcreteListener, typename... Args>
        requires std::is_base_of_v<Listener, ConcreteListener>
    auto addListener(Args&&... args) -> std::shared_ptr<ConcreteListener>;

    /**
     * @brief   Add a listener of the specified type.
     *
     * @tparam  ConcreteListener  Listener type.
     * @tparam  ListenerMutator   Listener mutator type.
     * @tparam  Args              Argument types.
     * @param   args              Arguments.
     * @param   mutate            Listener mutator.
     */

    template<typename ConcreteListener, typename ListenerMutator, typename... Args>
        requires std::is_base_of_v<Listener, ConcreteListener>
              && std::is_invocable_v<ListenerMutator, ConcreteListener&>
    auto addListener(Args&&... args, ListenerMutator&& mutate) -> std::shared_ptr<ConcreteListener>;

    /**
     * @brief   Invokes the specified action for all listeners.
     *
     * @tparam  FuncArgs  Argument types (signature).
     * @tparam  RealArgs  Argument types (real).
     * @param   action    Action.
     * @param   args      Arguments.
     */

    template<typename... FuncArgs, typename... RealArgs>
    auto invoke(auto (Listener::*action)(FuncArgs...), RealArgs&&... args) -> void;

    /**
     * @brief   Invokes the specified action within one listener.
     *
     * @tparam  FuncArgs  Argument types (signature).
     * @tparam  RealArgs  Argument types (real).
     * @param   action    Action.
     * @param   args      Arguments.
     */

    template<typename... FuncArgs, typename... RealArgs>
    auto invokeOnce(auto (Listener::*action)(FuncArgs...), RealArgs&&... args) -> void;

    /**
     * @brief   Removes the given listener.
     *
     * @tparam  ConcreteListener  Listener type.
     * @param   listener          Listener.
     *
     * @return  If listener has been removed - true, otherwise - false.
     */

    template<typename ConcreteListener>
        requires std::is_base_of_v<Listener, ConcreteListener>
    auto removeListener(const std::shared_ptr<ConcreteListener>& listener) -> bool;

    /**
     * @brief   Removes all listeners.
     */

    auto removeListeners() -> void;

    /**
     * @brief   Add the given listener.
     *
     * @tparam  ConcreteListener  Listener type.
     * @param   listener          Listener.
     */

    template<typename ConcreteListener>
        requires std::is_base_of_v<Listener, ConcreteListener>
    auto operator+=(std::shared_ptr<ConcreteListener> listener) -> void;

    /**
     * @brief   Remove the given listener.
     *
     * @tparam  ConcreteListener  Listener type.
     * @param   listener          Listener.
     *
     * @return  If listener has been removed - true, otherwise - false.
     */

    template<typename ConcreteListener>
        requires std::is_base_of_v<Listener, ConcreteListener>
    auto operator-=(const std::shared_ptr<ConcreteListener>& listener) -> bool;

protected:
    Listeners m_listeners {};
};

template<typename Listener>
auto Listenable<Listener>::hasListeners() const noexcept -> bool
{
    return !m_listeners.empty();
}

template<typename Listener>
template<typename ConcreteListener>
    requires std::is_base_of_v<Listener, ConcreteListener>
auto Listenable<Listener>::hasListener(std::shared_ptr<ConcreteListener>& listener) const noexcept -> bool
{
    return std::any_of(m_listeners.cbegin(), m_listeners.cend(),
        [&](const std::shared_ptr<Listener>& added_listener)
            {
                return added_listener == std::static_pointer_cast<Listener>(listener);
            });
}

template<typename Listener>
template<typename ConcreteListener>
    requires std::is_base_of_v<Listener, ConcreteListener>
auto Listenable<Listener>::addListener(std::shared_ptr<ConcreteListener> listener) -> void
{
    if (!listener)
    {
        throw InvalidArgumentException("Listener is null");
    }

    m_listeners.push_back(std::move(listener));
}

template<typename Listener>
template<typename ConcreteListener, typename ListenerMutator>
    requires std::is_base_of_v<Listener, ConcreteListener>
          && std::is_invocable_v<ListenerMutator, ConcreteListener&>
auto Listenable<Listener>::addListener(std::shared_ptr<ConcreteListener> listener, ListenerMutator&& mutate) -> void
{
    if (!listener)
    {
        throw InvalidArgumentException("Listener is null");
    }

    std::forward<ListenerMutator>(mutate)(*listener);
    m_listeners.push_back(std::move(listener));
}

template<typename Listener>
template<typename ConcreteListener, typename... Args>
    requires std::is_base_of_v<Listener, ConcreteListener>
auto Listenable<Listener>::addListener(Args&&... args) -> std::shared_ptr<ConcreteListener>
{
    auto listener = std::make_shared<ConcreteListener>(std::forward<Args>(args)...);
    auto listener_cp { listener };

    m_listeners.push_back(std::move(listener));
    return listener_cp;
}

template<typename Listener>
template<typename ConcreteListener, typename ListenerMutator, typename... Args>
    requires std::is_base_of_v<Listener, ConcreteListener>
          && std::is_invocable_v<ListenerMutator, ConcreteListener&>
auto Listenable<Listener>::addListener(Args&&... args, ListenerMutator&& mutate) -> std::shared_ptr<ConcreteListener>
{
    auto listener = std::make_shared<ConcreteListener>(std::forward<Args>(args)...);
    auto listener_cp { listener };

    std::forward<ListenerMutator>(mutate)(*listener);
    m_listeners.push_back(std::move(listener));

    return listener_cp;
}

template<typename Listener>
template<typename... FuncArgs, typename... RealArgs>
auto Listenable<Listener>::invoke(auto (Listener::*action)(FuncArgs...), RealArgs&&... args) -> void
{
    if (hasListeners())
    {
        for (auto& listener : m_listeners)
        {
            try
            {
                (*listener.*action)(std::forward<RealArgs>(args)...);
            }
            catch (...)
            {
                // Do nothing
            }
        }
    }
}

template<typename Listener>
template<typename... FuncArgs, typename... RealArgs>
auto Listenable<Listener>::invokeOnce(auto (Listener::*action)(FuncArgs...), RealArgs&&... args) -> void
{
    if (hasListeners())
    {
        try
        {
            (m_listeners.front().get()->*action)(std::forward<RealArgs>(args)...);
        }
        catch (...)
        {
            // Do nothing
        }
    }
}

template<typename Listener>
template<typename ConcreteListener>
    requires std::is_base_of_v<Listener, ConcreteListener>
auto Listenable<Listener>::removeListener(const std::shared_ptr<ConcreteListener>& listener) -> bool
{
    const auto prev_size = m_listeners.size();
    const auto begin =
        std::remove_if(m_listeners.begin(), m_listeners.end(), [&](const std::shared_ptr<Listener>& added_listener)
            {
                return added_listener == std::static_pointer_cast<Listener>(listener);
            });

    m_listeners.erase(begin, m_listeners.cend());
    return m_listeners.size() < prev_size;
}

template<typename Listener>
auto Listenable<Listener>::removeListeners() -> void
{
    m_listeners.clear();
}

template<typename Listener>
template<typename ConcreteListener>
    requires std::is_base_of_v<Listener, ConcreteListener>
auto Listenable<Listener>::operator+=(std::shared_ptr<ConcreteListener> listener) -> void
{
    addListener(std::move(listener));
}

template<typename Listener>
template<typename ConcreteListener>
    requires std::is_base_of_v<Listener, ConcreteListener>
auto Listenable<Listener>::operator-=(const std::shared_ptr<ConcreteListener>& listener) -> bool
{
    return removeListener(listener);
}

} // namespace isml

#endif // ISML_LISTENABLE_HPP
