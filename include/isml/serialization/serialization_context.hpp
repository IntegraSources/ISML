/**
 * @file    serialization_context.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    6/7/21
 */

#ifndef ISML_SERIALIZATION_CONTEXT_HPP
#define ISML_SERIALIZATION_CONTEXT_HPP

#include <typeinfo>
#include <memory>
#include <functional>

#include <isml/exceptions.hpp>
#include <isml/serialization/serializer_traits.hpp>

namespace isml {

/**
 * @class   SerializationContext
 * @brief   Provide details about the required serializer and give an access to
 *          the input/output object.
 * @since   0.1.4
 */

class SerializationContext
{
public:
    SerializationContext() = delete;
    SerializationContext(const SerializationContext&) = delete;

    virtual ~SerializationContext() = default;

    auto operator=(const SerializationContext&) -> SerializationContext& = delete;

protected:
    template<typename Stream>
    explicit SerializationContext(const std::type_info& serializer_type, Stream& stream) noexcept;

public:
    auto serializerTag() const noexcept -> const std::type_info&;

    /**
     * @brief   Create serialization context for the specified serializer.
     *
     * @tparam  Serializer  Serializer type.
     * @tparam  Object      IO object type.
     * @param   io          IO object.
     *
     * @return  Serialization context.
     */

    template<template<typename...> typename Serializer, typename Object>
        requires std::is_same_v<Object, typename SerializerTraits<Serializer>::Input>
              or std::is_same_v<Object, typename SerializerTraits<Serializer>::Output>
    static auto create(Object& io) noexcept -> SerializationContext;

    /**
     * @brief   Returns the input/output object with casting to the specified type.
     *
     * @tparam  Stream  IO object type.
     *
     * @return  A reference to the IO object.
     */

    template<typename Stream>
    auto stream() -> Stream&;

protected:
    std::reference_wrapper<const std::type_info> m_serializer_type;
    std::reference_wrapper<const std::type_info> m_io_type;
    void*                                        m_io;
};

template<typename Object>
inline SerializationContext::SerializationContext(const std::type_info& serializer_type, Object& io) noexcept
    : m_serializer_type(serializer_type)
    , m_io_type(typeid(Object))
    , m_io(std::addressof(io))
{}

template<template<typename...> typename Serializer, typename Object>
    requires std::is_same_v<Object, typename SerializerTraits<Serializer>::Input>
          or std::is_same_v<Object, typename SerializerTraits<Serializer>::Output>
inline auto SerializationContext::create(Object& io) noexcept -> SerializationContext
{
    return SerializationContext { typeid(SerializerTag<Serializer>), io };
}

template<typename Stream>
auto SerializationContext::stream() -> Stream&
{
    if (typeid(Stream) != m_io_type)
        throw InvalidCastException("Invalid cast");

    return *reinterpret_cast<Stream*>(m_io);
}

} // namespace isml

#endif // ISML_SERIALIZATION_CONTEXT_HPP
