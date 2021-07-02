cmake_minimum_required(VERSION 3.15)
project(isml VERSION 0.1.6 LANGUAGES CXX)

option(WITH_TESTS "Build library with unit tests" OFF)
option(WITH_EXAMPLES "Build library with examples" OFF)
option(WITH_DOCS "Build library with docs" OFF)

set(ISML_CORE "${PROJECT_NAME}.core")

add_definitions(-include ${CMAKE_CURRENT_SOURCE_DIR}/include/isml/platform.hpp)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

add_subdirectory(src)

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

if(WITH_EXAMPLES)
    add_subdirectory(examples)
endif()

if(WITH_DOCS)
    add_subdirectory(docs)
endif()
