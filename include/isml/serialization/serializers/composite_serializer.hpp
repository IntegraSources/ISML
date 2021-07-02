/**
 * @file    composite_serializer.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    6/9/21
 */

#ifndef ISML_COMPOSITE_SERIALIZER_HPP
#define ISML_COMPOSITE_SERIALIZER_HPP

#include <typeinfo>

#include <isml/exceptions.hpp>
#include <isml/serialization/serialization_context.hpp>

namespace isml {

/**
 * @class   CompositeSerializer
 * @brief   Used to compose serializers.
 * @tparam  Serializers
 * @since   0.1.4
 */

template<template<typename...> typename... Serializers>
class CompositeSerializer
{
public:
    template<typename T>
    static auto serialize(SerializationContext& context, const T& value, const std::string& name) -> void;

    template<typename T>
    static auto deserialize(SerializationContext& context, T& value, const std::string& name) -> void;

protected:
    static auto supportedBy(SerializationContext& context) noexcept -> bool;

    template<template<typename...> typename Serializer, typename T>
    static auto serializeUsing(SerializationContext& context, const T& value, const std::string& name) -> void;

    template<template<typename...> typename Serializer, typename T>
    static auto deserializeUsing(SerializationContext& context, T& value, const std::string& name) -> void;
};

template<typename T>
struct IsCompositeSerializer : std::false_type {};

template<template<typename...> typename... Ts>
struct IsCompositeSerializer<CompositeSerializer<Ts...>> : std::true_type {};

template<template<typename...> typename... Serializers>
template<typename T>
auto CompositeSerializer<Serializers...>::serialize(SerializationContext& context, const T& value, const std::string& name) -> void
{
    if (!supportedBy(context))
        throw IOException("Serializer is not supported");

    (serializeUsing<Serializers>(context, value, name), ...);
}

template<template<typename...> typename... Serializers>
template<typename T>
auto CompositeSerializer<Serializers...>::deserialize(SerializationContext& context, T& value, const std::string& name) -> void
{
    if (!supportedBy(context))
        throw IOException("Serializer is not supported");

    (deserializeUsing<Serializers>(context, value, name), ...);
}

template<template<typename...> typename... Serializers>
inline auto CompositeSerializer<Serializers...>::supportedBy(SerializationContext& context) noexcept -> bool
{
    return ((typeid(SerializerTag<Serializers>) == context.serializerTag()) || ...);
}

template<template<typename...> typename... Serializers>
template<template<typename...> typename Serializer, typename T>
inline auto CompositeSerializer<Serializers...>::serializeUsing(SerializationContext& context, const T& value, const std::string& name) -> void
{
    if (context.serializerTag() == typeid(SerializerTag<Serializer>))
        Serializer<T>::serialize(context, value, name);
}

template<template<typename...> typename... Serializers>
template<template<typename...> typename Serializer, typename T>
inline auto CompositeSerializer<Serializers...>::deserializeUsing(SerializationContext& context, T& value, const std::string& name) -> void
{
    if (context.serializerTag() == typeid(SerializerTag<Serializer>))
        Serializer<T>::deserialize(context, value, name);
}

} // namespace isml

#endif // ISML_COMPOSITE_SERIALIZER_HPP
