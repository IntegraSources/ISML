/**
 * @file    property_set.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    7/23/20
 */

#ifndef ISML_PROPERTY_SET_HPP
#define ISML_PROPERTY_SET_HPP

#include <unordered_map>
#include <queue>

#include <isml/utility/property.hpp>

namespace isml {

/**
 * @class   PropertySet
 * @brief   Represents property set with suitable interface to manipulate them.
 *
 * @tparam  K  The property name/key type.
 */

template<typename K>
class PropertySet
{
public:
    using Key = K;

public:
    PropertySet() = default;

public:
    auto operator[](const Key& key) noexcept -> Property&;

    auto contains(const Key& key) const noexcept -> bool;

    auto remove(const Key& key) noexcept -> bool;

    auto removeAll() -> void;

protected:
    std::unordered_map<Key, Property> m_properties {}; ///< Properties.
};

template<typename K>
inline auto PropertySet<K>::operator[](const Key& key) noexcept -> Property&
{
    return m_properties[key];
}

template<typename K>
inline auto PropertySet<K>::contains(const Key& key) const noexcept -> bool
{
    return m_properties.contains(key);
}

template<typename K>
inline auto PropertySet<K>::remove(const Key& key) noexcept -> bool
{
    return m_properties.erase(key) > 0;
}

template<typename K>
inline auto PropertySet<K>::removeAll() -> void
{
    m_properties.clear();
}

} // namespace isml

#endif // ISML_PROPERTY_SET_HPP
