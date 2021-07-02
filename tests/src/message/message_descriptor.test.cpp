/**
 * @file    message_descriptor.test.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    20.03.2020
 */

#include <memory>

ISML_DISABLE_WARNINGS_PUSH
#   include <gtest/gtest.h>
ISML_DISABLE_WARNINGS_POP

#include <isml/serialization/serializers/composite_serializer.hpp>
#include <isml/serialization/serializers/binary_serializer.hpp>

#include <isml/message/message_descriptor.hpp>

using namespace isml;

using FieldSerializer = CompositeSerializer<BinarySerializer>;

namespace { enum TestMessageType : MessageType { A, B }; }

TEST(MessageDescriptorTests, AddField)
{
    using namespace isml;

    const auto name = "field_name";
    MessageDescriptor descriptor { TestMessageType::A };
    descriptor.registerField<FieldSerializer, int>(name);
}

TEST(MessageDescriptorTests, MoveDescriptor)
{
    using namespace isml;

    const auto name = "field_name";
    MessageDescriptor descriptor { TestMessageType::A };
    descriptor.registerField<FieldSerializer, int>(name);
    MessageDescriptor descriptor2 { std::move(descriptor) };
}
