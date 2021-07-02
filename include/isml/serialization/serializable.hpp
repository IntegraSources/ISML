/**
 * @file    serializable.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    6/7/21
 */

#ifndef ISML_SERIALIZABLE_HPP
#define ISML_SERIALIZABLE_HPP

#include <isml/serialization/serialization_context.hpp>

namespace isml {

/**
 * @class   Serializable
 * @brief   Defines an interface for serialization objects.
 * @since   0.1.0
 */

class Serializable
{
public:
    virtual ~Serializable() = default;

public:

    /**
     * @brief   Translates object state to the byte stream (in general case) using the
     *          serializer specified in the given context.
     *
     * @param   context  Serialization context.
     */

    virtual auto serialize(SerializationContext& context) const -> void = 0;

    /**
     * @brief   Translates data from the byte stream (in general case) to the object state
     *          using the serializer specified in the given context.
     *
     * @param   context  Serialization context.
     */

    virtual auto deserialize(SerializationContext& context) -> void = 0;
};

} // namespace isml

#endif // ISML_SERIALIZABLE_HPP
