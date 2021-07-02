include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES gtest/cci.20210126
    OPTIONS    gtest:build_gmock=True
    GENERATORS cmake)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
    SETTINGS ${settings}
    BUILD missing)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(ISML_TESTS "${PROJECT_NAME}.tests")

add_executable(${ISML_TESTS})

target_compile_options(${ISML_TESTS} PRIVATE
    -std=c++2a -Wall -pedantic -Wextra -Weffc++ -Wno-unknown-pragmas)

target_include_directories(${ISML_TESTS} PRIVATE
    ${CONAN_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_sources(${ISML_TESTS} PRIVATE
    main.cpp
    messaging_service.tests.cpp
    # Base
    base/listenable.tests.cpp
    # Message - fields
    message/field/field.tests.cpp
    # Message
    message/message.tests.cpp
    message/message_descriptor.test.cpp
    message/message_factory.test.cpp
    # Net
    net/url.tests.cpp
    # Utility
    utility/properties.tests.cpp)

target_link_directories(${ISML_TESTS} PRIVATE
    ${CONAN_LIB_DIRS})

target_link_libraries(${ISML_TESTS} PRIVATE
    ${CONAN_LIBS}
    ${ISML_CORE})
