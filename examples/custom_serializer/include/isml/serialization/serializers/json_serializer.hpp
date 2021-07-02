/**
 * @file    json_serializer.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    6/7/21
 */

#ifndef ISML_JSON_SERIALIZER_HPP
#define ISML_JSON_SERIALIZER_HPP

#include <type_traits>
#include <cstdint>
#include <string>

ISML_DISABLE_WARNINGS_PUSH
#   include <json/json.h>
ISML_DISABLE_WARNINGS_POP

#include <isml/serialization/serializer_traits.hpp>
#include <isml/serialization/serialization_context.hpp>

class JsonSerializerBase
{
protected:
    static auto checkMember(const Json::Value& json, const std::string& name) -> void
    {
        if (!json.isMember(name))
            throw std::runtime_error("Member not found");
    }
};

template<typename T, typename P = void>
class JsonSerializer;

template<>
struct isml::SerializerTraits<JsonSerializer>
{
    using Output = Json::Value;
    using Input = Json::Value;
};

template<typename T>
class JsonSerializer<T, std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, bool>>>
    : protected JsonSerializerBase
{
public:
    static auto serialize(isml::SerializationContext& context, const T& value, const std::string& name) -> void
    {
        auto& json = context.stream<Json::Value>();
        json[name] = value;
    }

    static auto deserialize(isml::SerializationContext& context, T& value, const std::string& name) -> void
    {
        auto& json = context.stream<Json::Value>();
        checkMember(json, name);

        if constexpr (std::is_signed_v<T>)
        {
            value = static_cast<T>(json[name].asInt64());
        }
        else
        {
            value = static_cast<T>(json[name].asUInt64());
        }
    }
};

template<typename T>
class JsonSerializer<T, std::enable_if_t<std::is_same_v<T, std::string>>>
    : public JsonSerializerBase
{
public:
    static auto serialize(isml::SerializationContext& context, const T& value, const std::string& name) -> void
    {
        auto& json = context.stream<Json::Value>();
        json[name] = value;
    }

    static auto deserialize(isml::SerializationContext& context, T& value, const std::string& name) -> void
    {
        auto& json = context.stream<Json::Value>();
        checkMember(json, name);
        value = json[name].asString();
    }
};

template<typename T>
class JsonSerializer<T, std::enable_if_t<std::is_base_of_v<isml::Serializable, T>>>
    : public JsonSerializerBase
{
public:
    static auto serialize(isml::SerializationContext& context, const T& value, const std::string& name) -> void
    {
        auto subcontext =
            isml::SerializationContext::create<JsonSerializer>(context.stream<Json::Value>()[name]);
        value.serialize(subcontext);
    }

    static auto deserialize(isml::SerializationContext& context, T& value, const std::string& name) -> void
    {
        auto& json = context.stream<Json::Value>();
        checkMember(json, name);
        auto subcontext = isml::SerializationContext::create<JsonSerializer>(json[name]);
        value.deserialize(subcontext);
    }
};

#endif // ISML_JSON_SERIALIZER_HPP
