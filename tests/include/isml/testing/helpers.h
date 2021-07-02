/**
 * @file    helpers.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    26.02.2020
 */

#ifndef ISML_HELPERS_H
#define ISML_HELPERS_H

namespace isml {

class Copyable {};

class Movable {};

class NonCopyable
{
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    auto operator=(const NonCopyable&) -> NonCopyable& = delete;
};

class NonCopyableButMovable
{
public:
    NonCopyableButMovable(const NonCopyable&) = delete;
    auto operator=(const NonCopyableButMovable&) -> NonCopyableButMovable& = delete;
    NonCopyableButMovable(NonCopyableButMovable&&) = default;
    auto operator=(NonCopyableButMovable&&) -> NonCopyableButMovable& = default;
};

class NonMovable
{
public:
    NonMovable() = default;
    NonMovable(NonMovable&&) = delete;
    auto operator=(NonMovable&&) -> NonMovable& = delete;
};

} // namespace isml

#endif // ISML_HELPERS_H
