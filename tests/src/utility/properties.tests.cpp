/**
 * @file    properties.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    27.02.2020
 */

#include <string>

ISML_DISABLE_WARNINGS_PUSH
#   include <gtest/gtest.h>
ISML_DISABLE_WARNINGS_POP

#include <isml/utility/property_set.hpp>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

using Properties = isml::PropertySet<std::string>;

TEST(PropertiesTest, CommomOperations)
{
    constexpr auto key = "a";
    Properties properties;
    properties[key] = 10;
    ASSERT_TRUE(properties.contains(key));
    ASSERT_TRUE(properties[key].as<int>() == 10);
    properties.remove(key);
    ASSERT_FALSE(properties.contains(key));
    ASSERT_TRUE(properties[key].valueless());
}

#pragma clang diagnostic pop
