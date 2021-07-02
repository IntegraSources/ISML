/**
 * @file    field_set.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    01.03.2021
 */

#ifndef ISML_FIELD_SET_HPP
#define ISML_FIELD_SET_HPP

#include <unordered_map>
#include <list>

#include <isml/base/maybe.hpp>

#include <isml/serialization/serializable.hpp>

#include <isml/message/field/field.hpp>
#include <isml/message/field/value_field.hpp>

namespace isml {

/**
 * @class   FieldSet
 * @brief   Represents a set of message fields.
 * @since   0.1.0
 */

class FieldSet : public Serializable
{
protected:
    using Fields = std::list<Field::Ptr>;
    using FieldsByName = std::unordered_map<std::string, Fields::iterator>;

public:
    FieldSet() = default;
    FieldSet(const FieldSet& other);
    FieldSet(FieldSet&& other) noexcept;

    virtual ~FieldSet() = default;

    auto operator=(const FieldSet& other) -> FieldSet&;
    auto operator=(FieldSet&& other) -> FieldSet&;

public:
    auto add(Field::Ptr field) -> bool;

    auto contains(const std::string& name) const noexcept -> bool;

    template<typename T>
    auto contains(const std::string& name) const noexcept -> bool;

    auto get(const std::string& name) -> Maybe<Field::Ptr&>;

    template<typename T>
    auto get(const std::string& name) -> Maybe<ValueField<T>&>;

    auto empty() const noexcept -> bool;

    auto size() const noexcept -> std::size_t;

    auto assign(const FieldSet& other) -> void;

    auto swap(FieldSet& other) noexcept -> void;

    auto clear() -> void;

    auto serialize(SerializationContext& context) const -> void override;

    auto deserialize(SerializationContext& context) -> void override;

    auto serializedSize() const noexcept -> std::size_t;

protected:
    Fields       m_fields         {};
    FieldsByName m_fields_by_name {};
};

template<typename T>
auto FieldSet::contains(const std::string& name) const noexcept -> bool
{
    if (!m_fields_by_name.contains(name))
        return false;

    const auto& field = *m_fields_by_name.at(name);
    return field->valueType() == typeid(T);
}

template<typename T>
auto FieldSet::get(const std::string& name) -> Maybe<ValueField<T>&>
{
    Maybe<ValueField<T>&> field { none };
    if (contains<T>(name))
    {
        auto& field_ptr = *m_fields_by_name.at(name);
        field = *reinterpret_cast<ValueField<T>*>(field_ptr.get());
    }
    return field;
}

} // namespace isml

#endif // ISML_FIELD_SET_HPP
