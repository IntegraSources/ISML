/**
 * @file    field_set.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    01.03.2021
 */

#include <isml/message/field/field_set.hpp>

#include <utility>
#include <iterator>

namespace isml {

FieldSet::FieldSet(const FieldSet& other)
{
    assign(other);
}

FieldSet::FieldSet(FieldSet&& other) noexcept
{
    swap(other);
}

auto FieldSet::operator=(const FieldSet& other) -> FieldSet&
{
    if (this != &other)
    {
        assign(other);
    }

    return *this;
}

auto FieldSet::operator=(FieldSet&& other) -> FieldSet&
{
    if (this != &other)
    {
        swap(other);
    }

    return *this;
}

auto FieldSet::add(Field::Ptr field) -> bool
{
    if (m_fields_by_name.contains(field->name()))
        return false;

    const auto& name = field->name();
    auto it = m_fields.empty() ? m_fields.begin() : std::prev(m_fields.end());
    it = m_fields.insert(it, std::move(field));
    m_fields_by_name.insert(std::make_pair(name, it));
    return true;
}

auto FieldSet::contains(const std::string& name) const noexcept -> bool
{
    return m_fields_by_name.contains(name);
}

auto FieldSet::get(const std::string& name) -> Maybe<Field::Ptr&>
{
    Maybe<Field::Ptr&> field { none };
    if (m_fields_by_name.contains(name))
        field = *m_fields_by_name.at(name);
    return field;
}

auto FieldSet::empty() const noexcept -> bool
{
    return m_fields.empty();
}

auto FieldSet::size() const noexcept -> std::size_t
{
    return m_fields.size();
}

auto FieldSet::assign(const FieldSet& other) -> void
{
    clear();

    auto it = m_fields.begin();
    for (const auto& field : other.m_fields)
    {
        it = m_fields.insert(it, field->clone());
        m_fields_by_name.insert(std::make_pair(field->name(), it));
    }
}

auto FieldSet::swap(FieldSet& other) noexcept -> void
{
    std::swap(m_fields, other.m_fields);
    std::swap(m_fields_by_name, other.m_fields_by_name);
}

auto FieldSet::clear() -> void
{
    m_fields.clear();
    m_fields_by_name.clear();
}

auto FieldSet::serialize(SerializationContext& context) const -> void
{
    for (const auto& field : m_fields)
        field->serialize(context);
}

auto FieldSet::deserialize(SerializationContext& context) -> void
{
    for (auto& field : m_fields)
        field->deserialize(context);
}

auto FieldSet::serializedSize() const noexcept -> std::size_t
{
    std::size_t size = 0;
    for (const auto& field : m_fields)
        size += field->serializedSize();
    return size;
}

} // namespace isml
