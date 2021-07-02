/**
 * @file    url.tests.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    08.03.2021
 */

ISML_DISABLE_WARNINGS_PUSH
#   include <gtest/gtest.h>
ISML_DISABLE_WARNINGS_POP

#include <isml/net/url.hpp>

using namespace isml;

TEST(UrlTests, ParseString)
{
    try
    {
        const auto s = "protocol://domain/resource?param1=value1&param2=value2";
        const auto r = Url::parse(s);
        const auto u = r.toString();
        ASSERT_EQ(s, u);
    }
    catch (...)
    {
        FAIL();
    }

    try
    {
        const auto s = "protocol://domain:9000";
        const auto r = Url::parse(s);
        const auto u = r.toString();
        ASSERT_EQ(s, u);
    }
    catch (...)
    {
        FAIL();
    }

    try
    {
        const auto s = "vds://localhost/dev/ttyUSB0";
        const auto r = Url::parse(s);
        const auto u = r.toString();
        ASSERT_EQ(s, u);
    }
    catch (...)
    {
        FAIL();
    }
}
