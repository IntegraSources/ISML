/**
 * @file    field_descriptor.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    28/01/2020
 */

#ifndef ISML_FIELD_DESCRIPTOR_HPP
#define ISML_FIELD_DESCRIPTOR_HPP

#include <memory>

#include <isml/message/field/field.hpp>

namespace isml {

/**
 * @class   FieldDescriptor
 * @brief   Serves as a base class for other classes representing field descriptors.
 */

class FieldDescriptor
{
public:
    using Ptr = std::unique_ptr<FieldDescriptor>;

public:
    FieldDescriptor() = delete;
    FieldDescriptor(const FieldDescriptor& other);
    FieldDescriptor(FieldDescriptor&& other) noexcept;
    explicit FieldDescriptor(std::string name);

    virtual ~FieldDescriptor() = default;

    auto operator=(const FieldDescriptor&) -> FieldDescriptor& = delete;
    auto operator=(FieldDescriptor&&) -> FieldDescriptor& = delete;

public:
    /// Get the name of the field.
    auto name() const noexcept -> const std::string&;

    /**
     * @brief   Creates field.
     *
     * @return  A pointer to the created field.
     */

    virtual auto createField() const noexcept -> Field::Ptr = 0;

    /**
     * @brief   Creates a clone of the current descriptor.
     *
     * @return  A pointer to the created descriptor.
     */

    virtual auto clone() const -> Ptr = 0;

protected:
    const std::string m_name; ///< Field name.
};

} // namespace isml

#endif // ISML_FIELD_DESCRIPTOR_HPP
