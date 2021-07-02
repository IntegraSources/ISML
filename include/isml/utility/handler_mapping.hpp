/**
 * @file    handler_mapping.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/3/20
 */

#ifndef ISML_BASE_HANDLER_MAPPING_HPP
#define ISML_BASE_HANDLER_MAPPING_HPP

#include <functional>
#include <utility>
#include <unordered_map>
#include <type_traits>

#include <isml/utility/nullable_reference.hpp>

namespace isml {

template<typename T,
         typename Mapper = std::hash<T>,
         typename Key = std::invoke_result_t<Mapper, T>>
class HandlerMapping
{
public:
    using Handler = std::function<void(T)>;
    using HandlerRef = NullableReference<Handler>;
    using HandlerMap = std::unordered_map<Key, Handler>;

    static_assert(std::is_invocable_v<Mapper, T>);

public:
    HandlerMapping() = default;
    explicit HandlerMapping(Handler default_handler)
        noexcept(std::is_nothrow_move_constructible_v<Handler>);
    virtual ~HandlerMapping() = default;

public:
    auto addHandler(Key key, Handler handler)
        noexcept(std::is_nothrow_move_assignable_v<Handler>) -> void;

    auto setDefaultHandler(Handler handler)
        noexcept(std::is_nothrow_move_assignable_v<Handler>) -> void;

    template<typename U>
    auto handle(U object) -> void;

protected:
    virtual auto getHandler(const T& object) -> HandlerRef;

protected:
    Handler     m_default_handler {};
    HandlerMap  m_handlers        {};
};

template<typename T,
         typename Mapper,
         typename Key>
inline HandlerMapping<T, Mapper, Key>::HandlerMapping(Handler default_handler)
    noexcept(std::is_nothrow_move_constructible_v<Handler>)
    : m_default_handler(std::move(default_handler))
{}

template<typename T,
         typename Mapper,
         typename Key>
inline auto HandlerMapping<T, Mapper, Key>::addHandler(Key key, Handler handler)
    noexcept(std::is_nothrow_move_assignable_v<Handler>) -> void
{
    m_handlers[key] = std::move(handler);
}

template<typename T,
         typename Mapper,
         typename Key>
inline auto HandlerMapping<T, Mapper, Key>::setDefaultHandler(Handler handler)
    noexcept(std::is_nothrow_move_assignable_v<Handler>) -> void
{
    m_default_handler = std::move(handler);
}

template<typename T,
         typename Mapper,
         typename Key>
template<typename U>
auto HandlerMapping<T, Mapper, Key>::handle(U object) -> void
{
    if (auto handler = getHandler(object))
    {
        handler(std::forward<U>(object));
    }
}

template<typename T,
         typename Mapper,
         typename Key>
auto HandlerMapping<T, Mapper, Key>::getHandler(const T& object) -> HandlerRef
{
    const auto key = Mapper()(object);

    if (m_handlers.contains(key))
    {
        return HandlerRef { m_handlers.at(key) };
    }

    if (m_default_handler)
    {
        return HandlerRef { m_default_handler };
    }

    return nullref;
}

} // namespace isml

#endif // ISML_BASE_HANDLER_MAPPING_HPP
