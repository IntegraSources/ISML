/**
 * @file    serialization_utility.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    6/7/21
 */

#ifndef ISML_SERIALIZATION_UTILITY_HPP
#define ISML_SERIALIZATION_UTILITY_HPP

#include <isml/serialization/serializer_traits.hpp>
#include <isml/serialization/serializers/composite_serializer.hpp>
#include <isml/serialization/serialization_context.hpp>

namespace isml {

/**
 * @brief   Serialize the given value using the specified serializer.
 *
 * @tparam  Serializer  Serializer type.
 * @tparam  T           Value type.
 * @param   context     Serialization context.
 * @param   value       Value.
 * @param   name        Field name.
 *
 * @since   0.1.4
 */

template<template<typename...> typename Serializer, typename T>
auto serialize(SerializationContext& context, const T& value, const std::string& name) -> void
{
    Serializer<T>::serialize(context, value, name);
}

/**
 * @brief   Deserialize the given value using the specified serializer.
 *
 * @tparam  Serializer  Serializer type.
 * @tparam  T           Value type.
 * @param   context     Serialization context.
 * @param   value       Value.
 * @param   name        Field name.
 *
 * @since   0.1.4
 */

template<template<typename...> typename Serializer, typename T>
auto deserialize(SerializationContext& context, T& value, const std::string& name) -> void
{
    Serializer<T>::deserialize(context, value, name);
}

/**
 * @brief   Serialize the given value using the specified serializer.
 *          Specialization for the composed serializer.
 *
 * @tparam  Serializer  Serializer type.
 * @tparam  T           Value type.
 * @param   context     Serialization context.
 * @param   value       Value.
 * @param   name        Field name.
 *
 * @since   0.1.4
 */

template<typename Serializer, typename T>
    requires IsCompositeSerializer<Serializer>::value
auto serialize(SerializationContext& context, const T& value, const std::string& name) -> void
{
    Serializer::serialize(context, value, name);
}

/**
 * @brief   Deserialize the given value using the specified serializer.
 *          Specialization for the composed serializer.
 *
 * @tparam  Serializer  Serializer type.
 * @tparam  T           Value type.
 * @param   context     Serialization context.
 * @param   value       Value.
 * @param   name        Field name.
 *
 * @since   0.1.4
 */

template<typename Serializer, typename T>
    requires IsCompositeSerializer<Serializer>::value
auto deserialize(SerializationContext& context, T& value, const std::string& name) -> void
{
    Serializer::deserialize(context, value, name);
}

} // namespace isml

#endif // ISML_SERIALIZATION_UTILITY_HPP
