/**
 * @file    serializer_traits.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    6/7/21
 */

#ifndef ISML_SERIALIZER_TRAITS_HPP
#define ISML_SERIALIZER_TRAITS_HPP

namespace isml {

/**
 * @class   SerializerTag
 * @brief   A helper class used for matching serializers.
 * @tparam  Serializer Serializer type.
 * @since   0.1.4
 */

template<template<typename...> typename Serializer>
struct SerializerTag {};

/**
 * @brief   Serializer traits.
 *          Requires the definition of the type of the input object.
 *          Requires the definition of the type of the output object.
 *
 * @tparam  Serializer  Serializer type.
 *
 * @since   0.1.4
 */

template<template<typename...> typename Serializer>
struct SerializerTraits
{
    // typename Input
    // typename Output
};

} // namespace isml

#endif // ISML_SERIALIZER_TRAITS_HPP
