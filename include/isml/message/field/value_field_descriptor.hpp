/**
 * @file    value_field_descriptor.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    28/01/2020
 */

#ifndef ISML_VALUE_FIELD_DESCRIPTOR_HPP
#define ISML_VALUE_FIELD_DESCRIPTOR_HPP

#include <utility>

#include <isml/message/field/field_descriptor.hpp>
#include <isml/message/field/value_field.hpp>

namespace isml {

/**
 * @class   ValueFieldDescriptor
 * @tparam  T Value type.
 */

template<typename T, typename Serializer>
class ValueFieldDescriptor : public FieldDescriptor
{
public:
    using FieldDescriptor::FieldDescriptor;

public:
    /// @copydoc FieldDescriptor::createField()
    auto createField() const noexcept -> Field::Ptr override;

    /// @copydoc FieldDescriptor::clone()
    auto clone() const -> Ptr override;
};

template<typename T, typename Serializer>
inline auto ValueFieldDescriptor<T, Serializer>::createField() const noexcept -> Field::Ptr
{
    return std::make_unique<ValueField<T, Serializer>>(m_name);
}

template<typename T, typename Serializer>
auto ValueFieldDescriptor<T, Serializer>::clone() const -> FieldDescriptor::Ptr
{
    return std::make_unique<ValueFieldDescriptor>(*this);
}

} // namespace isml

#endif // ISML_VALUE_FIELD_DESCRIPTOR_HPP
