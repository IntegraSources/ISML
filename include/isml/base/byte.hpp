/**
 * @file    byte.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    26.02.2021
 */

#ifndef ISML_BYTE_HPP
#define ISML_BYTE_HPP

#include <isml/base/type_traits.hpp>

namespace isml {

class ByteUtils final
{
public:
    template<typename T>
        requires std::is_integral_v<T>
              && (sizeof(T) == sizeof(std::uint8_t))
    static auto swap(T&) noexcept -> void {}

    template<typename T>
        requires std::is_integral_v<T>
              && (sizeof(T) == sizeof(std::uint16_t))
    static auto swap(T& value) noexcept -> void;

    template<typename T>
        requires std::is_integral_v<T>
              && (sizeof(T) == sizeof(std::uint32_t))
    static auto swap(T& value) noexcept -> void;

    template<typename T>
        requires std::is_integral_v<T>
              && (sizeof(T) == sizeof(std::uint64_t))
    static auto swap(T& value) noexcept -> void;

    template<typename T>
        requires std::is_floating_point_v<T>
    static auto swap(T& value) noexcept -> void;

public:
    ByteUtils() = delete;
};

template<typename T>
    requires std::is_integral_v<T>
          && (sizeof(T) == sizeof(std::uint16_t))
auto ByteUtils::swap(T& value) noexcept -> void
{
    const auto c = static_cast<std::uint16_t>(value);
    const auto r = (c & 0xFF00U) >> 8
                 | (c & 0x00FFU) << 8;
    value = static_cast<T>(r);
}

template<typename T>
    requires std::is_integral_v<T>
          && (sizeof(T) == sizeof(std::uint32_t))
auto ByteUtils::swap(T& value) noexcept -> void
{
    const auto c = static_cast<std::uint32_t>(value);
    const auto r = (c & 0xFF000000U) >> 24
                 | (c & 0x00FF0000U) >> 8
                 | (c & 0x0000FF00U) << 8
                 | (c & 0x000000FFU) << 24;
    value = static_cast<T>(r);
}

template<typename T>
    requires std::is_integral_v<T>
          && (sizeof(T) == sizeof(std::uint64_t))
auto ByteUtils::swap(T& value) noexcept -> void
{
    const auto c = static_cast<std::uint64_t>(value);
    const auto r = (c & 0xFF00000000000000ULL) >> 56
                 | (c & 0x00FF000000000000ULL) >> 40
                 | (c & 0x0000FF0000000000ULL) >> 24
                 | (c & 0x000000FF00000000ULL) >> 8
                 | (c & 0x00000000FF000000ULL) << 8
                 | (c & 0x0000000000FF0000ULL) << 24
                 | (c & 0x000000000000FF00ULL) << 40
                 | (c & 0x00000000000000FFULL) << 56;
    value = static_cast<T>(r);
}

template<typename T>
    requires std::is_floating_point_v<T>
auto ByteUtils::swap(T& value) noexcept -> void
{
    const auto original_value = value;
    for (std::size_t i = 0; i < sizeof(T); ++i)
        reinterpret_cast<unsigned char*>(&value)[i] =
            reinterpret_cast<const unsigned char*>(&original_value)[sizeof(T) - 1 - i];
}

} // namespace isml

#endif // ISML_BYTE_HPP
