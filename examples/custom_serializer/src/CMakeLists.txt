include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES jsoncpp/1.9.4
    GENERATORS cmake)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
    SETTINGS ${settings}
    ENV CXXFLAGS=${CMAKE_CXX_FLAGS}
    BUILD missing)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(ISML_EXAMPLE "${PROJECT_NAME}.examples.custom_serializer")

add_executable(${ISML_EXAMPLE})

target_include_directories(${ISML_EXAMPLE} PRIVATE
    ${CONAN_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_sources(${ISML_EXAMPLE} PRIVATE
    main.cpp)

target_link_directories(${ISML_EXAMPLE} PRIVATE
    ${CONAN_LIB_DIRS})

target_link_libraries(${ISML_EXAMPLE} PRIVATE
    ${CONAN_LIBS}
    ${ISML_CORE})
