/**
 * @file    message_factory.test.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    20.03.2020
 */

ISML_DISABLE_WARNINGS_PUSH
#   include <gtest/gtest.h>
ISML_DISABLE_WARNINGS_POP

#include <isml/serialization/serializers/composite_serializer.hpp>
#include <isml/serialization/serializers/binary_serializer.hpp>

#include <isml/message/message_factory.hpp>

using namespace isml;

using FieldSerializer = CompositeSerializer<BinarySerializer>;

namespace { enum TestMessageType : MessageType { A, B }; }

TEST(MessageFactoryTests, AddMessageDescriptor)
{
    MessageFactory factory;
    factory.addDescriptor(TestMessageType::A, [](MessageDescriptor& descriptor)
        {
            descriptor.registerField<FieldSerializer, char>("a")
                      .registerField<FieldSerializer, long>("b");
        });

    ASSERT_TRUE(factory.hasDescriptor(TestMessageType::A));
}
