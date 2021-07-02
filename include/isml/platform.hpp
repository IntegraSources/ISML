/**
 * @file    platform.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    01.03.2021
 */

#ifndef ISML_PLATFORM_HPP
#define ISML_PLATFORM_HPP

#if defined(__clang__)
#   define ISML_COMPILER clang
#   define ISML_COMPILER_CLANG
#elif defined(__GNUC__)
#   define ISML_COMPILER GCC
#   define ISML_COMPILER_GCC
#endif

#define ISML_CONCAT_(ARG0, ARG1) ARG0 ## ARG1
#define ISML_CONCAT(ARG0, ARG1) ISML_CONCAT_(ARG0, ARG1)
#define ISML_STRINGIFY_(ARG) #ARG
#define ISML_STRINGIFY(ARG) ISML_STRINGIFY_(ARG)
#define ISML_SEPARATE_WITH_SPACE(ARG0, ARG1) ARG0 ARG1

#define ISML_COMPILER_SPECIFIC_PRAGMA_(ARG) _Pragma(ISML_STRINGIFY(ISML_SEPARATE_WITH_SPACE(ISML_COMPILER, ARG)))
#define ISML_COMPILER_SPECIFIC_PRAGMA(ARG) ISML_COMPILER_SPECIFIC_PRAGMA_(ARG)

#define ISML_DISABLE_WARNINGS_PUSH \
    ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic push) \
    ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wall") \
    ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wextra") \
    ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wpedantic") \
    ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Weffc++") \
    ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wdeprecated") \
    ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wnon-virtual-dtor") \
    ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wunused-parameter") \
    ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wconversion") \
    ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic ignored "-Wvolatile")

#define ISML_DISABLE_WARNINGS_POP \
    ISML_COMPILER_SPECIFIC_PRAGMA(diagnostic pop)

#endif // ISML_PLATFORM_HPP
