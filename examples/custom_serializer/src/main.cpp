/**
 * @file    main.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    6/7/21
 */

#include <iostream>
#include <future>

#include <isml/serialization/serializable.hpp>

#include <isml/serialization/serializers/composite_serializer.hpp>
#include <isml/serialization/serializers/binary_serializer.hpp>
#include <isml/serialization/serializers/json_serializer.hpp>
#include <isml/serialization/serialization_utility.hpp>

#include <isml/message/message_factory.hpp>
#include <isml/transport/tcp_transport_factory.hpp>
#include <isml/session/session_manager.hpp>

using namespace isml;

struct UserInfo : isml::Serializable
{
    std::string username {};
    unsigned    age      {};

    auto serialize(SerializationContext& context) const -> void override;
    auto deserialize(SerializationContext& context) -> void override;

    // Note: needed only for binary serialization
    auto serializedSize() const noexcept -> std::size_t;
};

enum ExampleMessageType : MessageType
{
    GetUserInfoRequest,
    GetUserInfoReply
};

// Step 1. Define custom serializer type using CompositeSerializer
using FieldSerializer = CompositeSerializer<BinarySerializer, JsonSerializer>;

auto main() -> int
{
    auto& message_factory = MessageFactory::getInstance();

    // Step 2. Specify the registered serializer type for message fields
    message_factory.addDescriptor(ExampleMessageType::GetUserInfoRequest,
        [](MessageDescriptor& descriptor)
            {
                descriptor.registerField<FieldSerializer, MessageId>("msgId");
            });

    message_factory.addDescriptor(ExampleMessageType::GetUserInfoReply,
        [](MessageDescriptor& descriptor)
            {
                descriptor.registerField<FieldSerializer, MessageId>("srcMsgId")
                          .registerField<FieldSerializer, UserInfo>("userInfo");
            });

    boost::asio::io_context ioc;
    TcpTransportFactory transport_factory { ioc };

    auto transport_res = transport_factory.createTransport(Url::parse("tcp://8.8.8.8:53"));
    if (!transport_res)
    {
        const auto& err = transport_res.error();
        std::cerr << "An error occurred while connecting to the server: " << err.message() << std::endl;
        return err.value();
    }

    auto service = std::async(std::launch::async, [&]{ ioc.run(); });

    SessionManager session_manager;
    auto session = session_manager.createSession(std::move(transport_res.value()));

    auto message =
        message_factory.createMessage(ExampleMessageType::GetUserInfoReply, *session);

    UserInfo ui;
    ui.username = "user";
    ui.age = 30;

    message->field<UserInfo>("userInfo").set(ui);

    {
        Json::Value json;
        auto context = SerializationContext::create<JsonSerializer>(json);
        message->serialize(context);

        std::cout << "JSON:\n" << json.toStyledString() << std::endl;
    }

    {
        std::stringstream stream;
        auto context = SerializationContext::create<BinarySerializer>(stream);
        message->serialize(context);

        std::cout << "Binary:\n" << stream.str() << std::endl;
    }

    session->shutdown();
    ioc.stop();
    service.wait();
    return 0;
}

auto UserInfo::serialize(SerializationContext& context) const -> void
{
    isml::serialize<FieldSerializer>(context, username, "username");
    isml::serialize<FieldSerializer>(context, age, "age");
}

auto UserInfo::deserialize(SerializationContext& context) -> void
{
    isml::deserialize<FieldSerializer>(context, username, "username");
    isml::deserialize<FieldSerializer>(context, age, "age");
}

auto UserInfo::serializedSize() const noexcept -> std::size_t
{
    std::size_t size = 0;
    size += binary::size(username);
    size += binary::size(age);
    return size;
}
