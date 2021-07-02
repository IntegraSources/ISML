/**
 * @file    binary_serializer.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    26.02.2021
 */

#ifndef ISML_BINARY_SERIALIZER_HPP
#define ISML_BINARY_SERIALIZER_HPP

#include <cassert>
#include <bit>

#include <numeric>
#include <cstddef>
#include <utility>
#include <sstream>
#include <concepts>

#include <isml/base/byte.hpp>
#include <isml/base/concepts.hpp>

#include <isml/serialization/serializable.hpp>
#include <isml/serialization/serializer_traits.hpp>

namespace isml {
namespace binary {

template<typename T>
auto serialize(SerializationContext& context, const T& value) -> void;

template<typename T>
auto deserialize(SerializationContext& context, T& value) -> void;

template<typename T>
auto size(const T& value) -> std::size_t;

template<typename T>
auto size() noexcept -> std::size_t;

} // namespace binary

template<typename T>
concept CanCalculateSelfBinarySize =
    std::is_base_of_v<Serializable, T> &&
    requires(const T& t)
    {
        { t.serializedSize() } -> std::same_as<std::size_t>;
    };

class BinarySerializerBase
{
public:
    using ContainerSize = std::uint16_t;

protected:
    ~BinarySerializerBase() = default;

protected:
    template<typename C>
    static auto size(const C& container) noexcept -> std::size_t
    {
        return std::accumulate(std::cbegin(container), std::cend(container), sizeof(ContainerSize),
            [](std::size_t sum, const auto& value)
                {
                    return sum + binary::size(value);
                });
    }

    template<typename T>
    static auto swapBytesIfNeeded(T& value) noexcept -> void
    {
        if constexpr (std::endian::native == std::endian::big)
        {
            ByteUtils::swap(value);
        }
    }

public:
    static constexpr std::uint64_t max_container_size = ~static_cast<ContainerSize>(0);
};

template<typename T, typename P = void>
class BinarySerializer;

template<typename T>
class BinarySerializer<T, std::enable_if_t<std::is_integral_v<T> or
                                           std::is_floating_point_v<T>>>
    : public BinarySerializerBase
{
public:
    static auto serialize(SerializationContext& context, T value, const std::string&) -> void
    {
        auto& stream = context.stream<std::stringstream>();
        swapBytesIfNeeded(value);
        stream.write(reinterpret_cast<const char*>(&value), sizeof value);
    }

    static auto deserialize(SerializationContext& context, T& value, const std::string&) -> void
    {
        auto& stream = context.stream<std::stringstream>();
        stream.read(reinterpret_cast<char*>(&value), sizeof value);
        swapBytesIfNeeded(value);
    }

    static auto size(const T&) noexcept -> std::size_t
    {
        return sizeof(T);
    }
};

template<typename T>
class BinarySerializer<T, std::enable_if_t<IsPair<T>::value>>
{
public:
    static auto serialize(SerializationContext& context, const T& pair, const std::string&) -> void
    {
        binary::serialize(context, pair.first);
        binary::serialize(context, pair.second);
    }

    static auto deserialize(SerializationContext& context, T& pair, const std::string&) -> void
    {
        binary::deserialize(context, pair.first);
        binary::deserialize(context, pair.second);
    }

    static auto size(const T& pair) noexcept -> std::size_t
    {
        std::size_t size = 0;
        size += binary::size(pair.first);
        size += binary::size(pair.second);
        return size;
    }
};

template<typename T>
class BinarySerializer<T, std::enable_if_t<IsOptional<T>::value>>
    : public BinarySerializerBase
{
public:
    static auto serialize(SerializationContext& context, const T& optional, const std::string&) -> void
    {
        const auto value_present = optional.has_value();
        swapBytesIfNeeded(value_present);
        binary::serialize(context, value_present);
        if (optional.has_value())
            binary::serialize(context, *optional);
    }

    static auto deserialize(SerializationContext& context, T& optional, const std::string&) -> void
    {
        bool value_present;
        binary::deserialize(context, value_present);
        if (value_present)
        {
            typename T::value_type value;
            binary::deserialize(context, value);
            optional = std::make_optional(std::move(value));
        }
    }

    static auto size(const T& optional) noexcept -> std::size_t
    {
        std::size_t size = 0;
        size += binary::size<bool>();
        if (optional.has_value())
            size += binary::size(*optional);
        return size;
    }
};

template<typename T>
class BinarySerializer<T, std::enable_if_t<SequenceContainer<T>>>
    : public BinarySerializerBase
{
public:
    static auto serialize(SerializationContext& context, const T& container, const std::string&) -> void
    {
        assert(container.size() <= max_container_size);
        binary::serialize(context, static_cast<ContainerSize>(container.size()));
        for (const auto& item : container)
            binary::serialize(context, item);
    }

    static auto deserialize(SerializationContext& context, T& container, const std::string&) -> void
    {
        ContainerSize item_count;
        binary::deserialize(context, item_count);
        container.resize(item_count);
        for (auto& item : container)
            binary::deserialize(context, item);
    }

    static auto size(const T& container) noexcept -> std::size_t
    {
        return BinarySerializerBase::size(container);
    }
};

template<typename T>
class BinarySerializer<T, std::enable_if_t<AssociativeContainer<T>>>
    : public BinarySerializerBase
{
public:
    static auto serialize(SerializationContext& context, const T& container, const std::string&) -> void
    {
        binary::serialize(context, static_cast<ContainerSize>(container.size()));
        for (const auto& item : container)
            binary::serialize(context, item);
    }

    static auto deserialize(SerializationContext& context, T& container, const std::string&) -> void
    {
        ContainerSize item_count;
        binary::deserialize(context, item_count);
        container.clear();
        if constexpr (Map<T>)
        {
            using Key = typename T::key_type;
            using Value = typename T::mapped_type;
            for (std::size_t i = 0; i < item_count; ++i)
            {
                Key key;
                binary::deserialize(context, key);
                Value value;
                binary::deserialize(context, value);
                container.insert(std::make_pair(std::move(key), std::move(value)));
            }
        }
        else
        {
            using Key = typename T::key_type;
            for (std::size_t i = 0; i < item_count; ++i)
            {
                Key key;
                binary::deserialize(context, key);
                container.insert(std::move(key));
            }
        }
    }

    static auto size(const T& container) noexcept -> std::size_t
    {
        return BinarySerializerBase::size(container);
    }
};

template<typename T>
class BinarySerializer<T, std::enable_if_t<std::is_enum_v<T>>>
    : public BinarySerializerBase
{
public:
    static auto serialize(SerializationContext& context, const T& enumerator, const std::string&) -> void
    {
        auto& stream = context.stream<std::stringstream>();
        auto value = static_cast<std::underlying_type_t<T>>(enumerator);
        swapBytesIfNeeded(value);
        stream.write(reinterpret_cast<const char*>(&value), sizeof value);
    }

    static auto deserialize(SerializationContext& context, T& enumerator, const std::string&) -> void
    {
        auto& stream = context.stream<std::stringstream>();
        std::underlying_type_t<T> value;
        stream.read(reinterpret_cast<char*>(&value), sizeof value);
        swapBytesIfNeeded(value);
        enumerator = static_cast<T>(value);
    }

    static auto size(const T&) noexcept -> std::size_t
    {
        return sizeof(std::underlying_type_t<T>);
    }
};

template<typename T>
class BinarySerializer<T, std::enable_if_t<IsArray<T>::value>>
    : public BinarySerializerBase
{
public:
    static auto serialize(SerializationContext& context, const T& array, const std::string&) -> void
    {
        binary::serialize(context, static_cast<ContainerSize>(array.size()));
        for (const auto& item : array)
            binary::serialize(context, item);
    }

    static auto deserialize(SerializationContext& context, T& array, const std::string&) -> void
    {
        ContainerSize item_count;
        binary::deserialize(context, item_count);
        assert(item_count == array.size());
        for (auto& item : array)
            binary::deserialize(context, item);
    }

    static auto size(const T& array) noexcept -> std::size_t
    {
        return BinarySerializerBase::size(array);
    }
};

template<typename T>
class BinarySerializer<T, std::enable_if_t<std::is_base_of_v<Serializable, T>>>
    : public BinarySerializerBase
{
public:
    static auto serialize(SerializationContext& context, const T& object, const std::string&) -> void
    {
        object.serialize(context);
    }

    static auto deserialize(SerializationContext& context, T& object, const std::string&) -> void
    {
        object.deserialize(context);
    }

    static auto size(const T& object) noexcept -> std::size_t requires CanCalculateSelfBinarySize<T>
    {
        return object.serializedSize();
    }
};

template<>
struct SerializerTraits<BinarySerializer>
{
    using Input = std::stringstream;
    using Output = std::stringstream;
};

namespace binary {

template<typename T>
inline auto serialize(SerializationContext& context, const T& value) -> void
{
    BinarySerializer<T>::serialize(context, value, "");
}

template<typename T>
inline auto deserialize(SerializationContext& context, T& value) -> void
{
    BinarySerializer<T>::deserialize(context, value, "");
}

template<typename T>
inline auto size(const T& value) -> std::size_t
{
    return BinarySerializer<T>::size(value);
}

template<typename T>
inline auto size() noexcept -> std::size_t
{
    return sizeof(T);
}

} // namespace binary
} // namespace isml

#endif // ISML_BINARY_SERIALIZER_HPP
