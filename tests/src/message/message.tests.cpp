/**
 * @file    message.tests.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    19.04.2020
 */

#include <sstream>

ISML_DISABLE_WARNINGS_PUSH
#   include <gtest/gtest.h>
ISML_DISABLE_WARNINGS_POP

#include <isml/serialization/serializers/composite_serializer.hpp>
#include <isml/serialization/serializers/binary_serializer.hpp>

#include <isml/message/message.hpp>
#include <isml/message/message_factory.hpp>

#include <isml/session/fake_session.hpp>

using namespace isml;

using FieldSerializer = CompositeSerializer<BinarySerializer>;

namespace { enum TestMessageType : MessageType { A, B }; }

TEST(MessageTests, Serialization)
{
    using namespace isml;

    MessageFactory factory;
    factory.addDescriptor(TestMessageType::A, [](MessageDescriptor& descriptor)
        {
            descriptor.registerField<FieldSerializer, int>("a")
                      .registerField<FieldSerializer, int>("b");
        });

    std::stringstream ss;

    Session::Ptr session { new FakeSession() };

    auto context = SerializationContext::create<BinarySerializer>(ss);
    auto msg1 = factory.createMessage(TestMessageType::A, *session);
    msg1->field<int>("a") = 10;
    msg1->field<int>("a") = 20;
    msg1->serialize(context);

    auto msg2 = factory.createMessage(TestMessageType::A, *session);
    msg2->deserialize(context);

    ASSERT_EQ(msg1->field<int>("a"), msg2->field<int>("a"));
    ASSERT_EQ(msg1->field<int>("b"), msg2->field<int>("b"));
}
