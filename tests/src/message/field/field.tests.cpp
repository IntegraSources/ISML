/**
 * @file	field.tests.cpp
 * @author	Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date	5/1/20
 */

ISML_DISABLE_WARNINGS_PUSH
#   include <gtest/gtest.h>
ISML_DISABLE_WARNINGS_POP

#include <isml/serialization/serializers/composite_serializer.hpp>
#include <isml/serialization/serializers/binary_serializer.hpp>

#include <isml/message/field/value_field.hpp>

using namespace isml;

using FieldSerializer = CompositeSerializer<BinarySerializer>;

TEST(FieldTests, CopyConstruction)
{
    ValueField<int, FieldSerializer> mf1 { "xxx" };
    ValueField<int, FieldSerializer> mf2 { mf1 };
    ASSERT_EQ(mf1, mf2);
}

TEST(FieldTests, MoveConstruction)
{
    ValueField<int, FieldSerializer> mf1 { "xxx" };
    mf1 = 10;
    ValueField<int, FieldSerializer> mf2 { std::move(mf1) };
    ASSERT_EQ(mf2.name(), "xxx");
    ASSERT_EQ(mf2.get(), 10);
}

TEST(FieldTests, Compare)
{
    ValueField<int, FieldSerializer> mf1 { "xxx" };
    ValueField<int, FieldSerializer> mf2 { "xxx" };
    ASSERT_EQ(mf1, mf2);
}

TEST(FieldTests, CompareNonEqualsFields)
{
    ValueField<int, FieldSerializer> mf1 { "xxx" };
    ValueField<int, FieldSerializer> mf2 { "yyy" };
    ASSERT_NE(mf1, mf2);
}

TEST(FieldTests, AssignValue)
{
    ValueField<int, FieldSerializer> mf1 { "xxx" };
    mf1 = 20;
    ASSERT_EQ(mf1.get(), 20);
}
