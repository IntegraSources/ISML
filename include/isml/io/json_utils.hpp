/**
 * @file    json_utils.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    12/03/2020
 */

#ifndef ISML_JSON_UTILS_HPP
#define ISML_JSON_UTILS_HPP

#include <string>
#include <fstream>

#include <isml/io/exceptions.hpp>
#include <isml/io/types.hpp>

namespace isml {

class JsonUtils final
{
public:
    static auto toString(const JsonObject& json) -> std::string
    {
        Json::StreamWriterBuilder builder;
        return Json::writeString(builder, json);
    }

    static auto load(const std::string& pathname) -> JsonObject
    {
        Json::CharReaderBuilder builder;
        JsonObject json;
        std::string errs;
        auto is = std::fstream(pathname);
        if (!Json::parseFromStream(builder, is, &json, &errs))
        {
            if (is.is_open()) is.close();
            throw IOException("An error occurred during JSON reading");
        }
        if (is.is_open()) is.close();
        return json;
    }

    static auto fromString(const std::string& str)
    {
        Json::CharReaderBuilder builder;
        JsonObject json;
        std::string errs;
        auto is = std::stringstream(str);
        if (!Json::parseFromStream(builder, is, &json, &errs))
        {
            throw IOException("An error occurred during JSON reading");
        }
        return json;
    }

    /**
     * @brief   Merges two JSON objects into one object.
     *
     * @param   lhs  First JSON object.
     * @param   rhs  Second JSON object.
     *
     * @return  If at least one of the specified objects is not a JSON object,
     *          the merge cannot be done, false will be returned,
     *          otherwise - true.
     */

    static auto merge(JsonObject& lhs, const JsonObject& rhs) -> bool
    {
        if (!lhs.isObject() or !rhs.isObject())
        {
            return false;
        }

        for (const auto& name : rhs.getMemberNames())
        {
            if (lhs.isMember(name) && lhs[name].isObject())
            {
                merge(lhs[name], rhs[name]);
            }
            else
            {
                lhs[name] = rhs[name];
            }
        }

        return true;
    }
};

} // namespace isml

#endif // ISML_JSON_UTILS_HPP
