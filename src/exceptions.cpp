/**
 * @file    exceptions.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    22.01.2020
 */

#include <isml/exceptions.hpp>

namespace isml {

Exception::Exception(std::string message) noexcept
    : m_message(std::move(message))
{}

auto Exception::message() const noexcept -> const std::string&
{
    return m_message;
}

auto Exception::what() const noexcept -> const char*
{
    return m_message.c_str();
}

SystemException::SystemException(std::string message, const std::error_code& ec) noexcept
    : Exception(std::move(message))
    , m_ec(ec)
{}

auto SystemException::code() const noexcept -> const std::error_code&
{
    return m_ec;
}

} // namespace isml
