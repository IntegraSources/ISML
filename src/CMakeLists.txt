include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES boost/1.76.0
                               fmt/7.1.3
    OPTIONS fmt:header_only=True
    GENERATORS cmake)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
    SETTINGS ${settings}
    ENV CXXFLAGS=${CMAKE_CXX_FLAGS}
    BUILD missing)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(${ISML_CORE})

target_include_directories(${ISML_CORE} PUBLIC
    ${CONAN_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_sources(${ISML_CORE} PRIVATE
    isml.cpp
    exceptions.cpp
    messaging_service.cpp
    # Executors
    executors/thread_pool_task_executor.cpp
    # Message
    message/channels/pubsub_message_channel.cpp
    message/field/field.cpp
    message/field/field_descriptor.cpp
    message/field/field_set.cpp
    message/filters/rule_based_message_filter.cpp
    message/exceptions.cpp
    message/message.cpp
    message/message_channel.cpp
    message/message_controller.cpp
    message/message_descriptor.cpp
    message/message_dispatcher.cpp
    message/message_factory.cpp
    message/message_filter_chain.cpp
    message/message_queue.cpp
    # Net
    net/url.cpp
    net/url_builder.cpp
    # Serialization
    serialization/serialization_context.cpp
    # Service
    service/service.cpp
    service/service_manager.cpp
    # Session
    session/session.cpp
    session/session_factory.cpp
    session/session_manager.cpp
    # System
    sys/signal_interceptor.cpp
    # Transport
    transport/tcp_transport.cpp
    transport/tcp_transport_factory.cpp
    transport/transport.cpp
    transport/transport_factory.cpp
    transport/transport_registry.cpp
    # Utility
    utility/stream_utils.cpp)

target_link_directories(${ISML_CORE} PUBLIC
    ${CONAN_LIB_DIRS})

target_link_libraries(${ISML_CORE} PUBLIC
    ${CONAN_LIBS})
