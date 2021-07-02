/**
 * @file    thread_joiner.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    10/26/20
 */

#ifndef ISML_THREAD_JOINER_HPP
#define ISML_THREAD_JOINER_HPP

#include <thread>
#include <type_traits>

#include <isml/utility/deferred_call.hpp>

namespace isml {

/**
 * @class   ThreadJoiner
 * @brief   Joins the specified thread(s) using the RAII idiom.
 */

class ThreadJoiner
    : public DeferredCall
{
public:
    ThreadJoiner() = delete;

    /**
     * @brief   Constructor.
     * @param   thread  Thread to be joined.
     */

    explicit ThreadJoiner(std::thread& thread);

    /**
     * @brief   Constructor.
     *
     * @tparam  Container  One of the standard sequence container storing objects
     *                     of the std::thread type.
     *
     * @param   container  Container.
     */

    template<typename Container>
    explicit ThreadJoiner(Container& container);

    // Non-copyable
    ThreadJoiner(const ThreadJoiner&) = delete;
    auto operator=(const ThreadJoiner&) -> ThreadJoiner& = delete;

protected:
    template<typename Thread>
        requires std::is_same_v<Thread, std::thread>
    static auto join(Thread& thread) -> void;

    template<typename Container>
        requires std::is_same_v<std::thread, typename Container::value_type>
    static auto join(Container& container) -> void;
};

inline ThreadJoiner::ThreadJoiner(std::thread& thread)
    : DeferredCall(defer(&ThreadJoiner::join<std::thread>, std::ref(thread)))
{}

template<typename Container>
inline ThreadJoiner::ThreadJoiner(Container& container)
    : DeferredCall(defer(&ThreadJoiner::join<Container>, std::ref(container)))
{}

template<typename Thread>
    requires std::is_same_v<Thread, std::thread>
auto ThreadJoiner::join(Thread& thread) -> void
{
    if (thread.joinable())
    {
        thread.join();
    }
    else
    {
        thread.detach();
    }
}

template<typename Container>
    requires std::is_same_v<std::thread, typename Container::value_type>
auto ThreadJoiner::join(Container& container) -> void
{
    for (auto& thread : container)
    {
        join(thread);
    }
}

} // namespace isml

#endif // ISML_THREAD_JOINER_HPP
