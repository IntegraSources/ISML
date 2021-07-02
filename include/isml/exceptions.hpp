/**
 * @file    exceptions.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    22.01.2020
 */

#ifndef ISML_EXCEPTIONS_HPP
#define ISML_EXCEPTIONS_HPP

#include <exception>
#include <string>
#include <string_view>
#include <system_error>

#define ISML_DEFINE_EXCEPTION(EXCEPTION_BASE, EXCEPTION) \
class EXCEPTION : public EXCEPTION_BASE                 \
{                                                       \
public:                                                 \
    EXCEPTION() = delete;                               \
    using EXCEPTION_BASE::EXCEPTION_BASE;               \
};

namespace isml {

/**
 * @class   Exception
 * @brief   Serves as the base class for project-defined exceptions.
 */

class Exception : public std::exception
{
public:
    explicit Exception(std::string message) noexcept;

public:

    /**
     * @brief   Gets a message describing occurred error.
     */

    auto message() const noexcept -> const std::string&;

    /**
     * @brief   Gets a message describing occurred error.
     */

    auto what() const noexcept  -> const char* final;

private:
    const std::string m_message; ///< Error message.
};

/**
 * @class   SystemException
 * @brief   Serves as a base class for all system exceptions.
 */

class SystemException : public Exception
{
public:
    using Exception::Exception;
    SystemException(std::string message, const std::error_code& ec) noexcept;

public:
    auto code() const noexcept -> const std::error_code&;

private:
    std::error_code m_ec {}; ///< Error code.
};

template<typename Exception, typename... Args>
    requires std::is_base_of_v<std::exception, Exception>
inline auto raise_ptr(Args&&... args) -> std::exception_ptr
{
    try
    {
        throw Exception(std::forward<Args>(args)...);
    }
    catch (...)
    {
        return std::current_exception();
    }
}

// Generalized exceptions
ISML_DEFINE_EXCEPTION(Exception, IOException)
ISML_DEFINE_EXCEPTION(Exception, LogicException)
ISML_DEFINE_EXCEPTION(Exception, RuntimeException)

/**
 * @class   MessageException
 * @brief   Serves as a base class for message exceptions.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, MessageException)

/**
 * @class   SessionException
 * @brief   Serves as a base class for session exceptions.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, SessionException)

/**
 * @class   NetworkException
 * @brief   Serves as a base class for all network-specific exceptions.
 */

ISML_DEFINE_EXCEPTION(SystemException, NetworkException)

/**
 * @class   NotImplementedException
 * @brief   The exception that is thrown when a requested method or operation
 *          is not implemented.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, NotImplementedException)

/**
 * @class  TypeDomainException
 * @brief  Indicates that value doesn't belong to the values domain of
 *         a specific type.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, TypeDomainException)

/**
 * @class  ValidationException
 * @brief  Indicates that the validation of some object was failed.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, ValidationException)

/**
 * @class   ConverterException
 * @brief   Indicates that the requested conversion cannot be performed.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, ConverterException)

/**
 * @class   NullPointerException
 * @brief   The exception that is thrown when there is an attempt to dereference
 *          a null pointer.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, NullPointerException)

/**
 * @class   InternalError
 * @brief   Indicates an internal error.
 */

ISML_DEFINE_EXCEPTION(Exception, InternalError)

/**
 * @class   ConfigurationException
 * @brief   Indicates that an error occurred during configuration loading/saving.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, ConfigurationException)

/**
 * @class   ExecutorException
 * @brief   Serves as a base class for all task executor related exceptions.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, ExecutorException)

/**
 * @class   InvalidCastException
 * @brief   The exception that is thrown for invalid casting or explicit conversion.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, InvalidCastException)

/**
 * @class   InvalidOperationException
 * @brief   The exception that is thrown when a method call is invalid for the object's current state.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, InvalidOperationException)

/**
 * @class   InvalidArgumentException
 * @brief   Thrown to indicate that a method has been passed an invalid
 *          or inappropriate argument.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, InvalidArgumentException)

/**
 * @class   FilesystemException
 * @brief   Serves as a base class for exceptions thrown on failed filesystem
 *          operations.
 */

ISML_DEFINE_EXCEPTION(IOException, FilesystemException)

/**
 * @class   FileNotFoundException
 * @brief   The exception that is thrown when an attempt to access a file that
 *          does not exist on disk fails.
 */

ISML_DEFINE_EXCEPTION(FilesystemException, FileNotFoundException)

/**
 * @class   SecurityException
 * @brief   Serves as a base class for all exception related to security issues.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, SecurityException)

/**
 * @class   DatabaseException
 * @brief   Serves as a base class for all exceptions thrown on database
 *          operation failures.
 */

ISML_DEFINE_EXCEPTION(RuntimeException, DatabaseException)

/**
 * @class   SqlException
 * @brief   Serves as a base class for all exceptions indicating errors occurred
 *          during creating or executing SQL queries.
 */

ISML_DEFINE_EXCEPTION(DatabaseException, SqlException)

/**
 * @class   MalformedUrlException
 * @brief   Indicates that malformed URL has occurred.
 */

ISML_DEFINE_EXCEPTION(IOException, MalformedUrlException)

/**
 * @class   TransportException
 * @brief   Indicates that an error occurred during transporting messages.
 */

ISML_DEFINE_EXCEPTION(IOException, TransportException)

/**
 * @class   TransportStateException
 * @brief   An exception thrown when the underlying transport is in a state that
 *          doesn't allow execution of the requested command.
 */

ISML_DEFINE_EXCEPTION(TransportException, TransportStateException)

inline auto failwith(std::string message) -> void
{
    throw RuntimeException(std::move(message));
}

template<typename E>
    requires std::is_base_of_v<Exception, E>
inline auto failwith(std::string message) -> void
{
    throw E(std::move(message));
}

} // namespace isml

#endif // ISML_EXCEPTIONS_HPP
