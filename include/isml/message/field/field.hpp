/**
 * @file    field.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    28/01/2020
 */

#ifndef ISML_FIELD_HPP
#define ISML_FIELD_HPP

#include <memory>
#include <string>
#include <typeinfo>

#include <isml/serialization/serializable.hpp>

namespace isml {

/**
 * @class   Field
 * @brief   Serves as a base class for other classes representing message fields.
 * @since   0.1.0
 */

class Field : public Serializable
{
public:
    using Ptr = std::unique_ptr<Field>;

public:
    Field() = delete;
    Field(const Field& other);
    Field(Field&&) = delete;
    explicit Field(std::string name) noexcept;

    virtual ~Field() = default;

    auto operator=(const Field&) -> Field& = delete;
    auto operator=(Field&&) -> Field& = delete;

public:

    /// Gets the name of the fields.
    auto name() const noexcept -> const std::string&;

    /**
     * @brief   Creates a clone of the current field.
     *
     * @return  A pointer to the created field.
     */

    virtual auto clone() const noexcept -> Ptr = 0;

    /**
     * @brief   Returns an info about the value type.
     *          Must be override by derived classes.
     *
     * @return  A result of invoking typeid() to the field type.
     */

    virtual auto valueType() const noexcept -> const std::type_info& = 0;

    /**
     * @brief   Returns the size of the serialized field.
     *          Used only for binary serialization.
     *
     * @return  The number of bytes.
     */

    virtual auto serializedSize() const noexcept -> std::size_t = 0;

protected:
    const std::string m_name;
};

} // namespace isml

#endif // ISML_FIELD_HPP
