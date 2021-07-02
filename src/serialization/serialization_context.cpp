/**
 * @file    serialization_context.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    6/7/21
 */

#include <isml/serialization/serialization_context.hpp>

namespace isml {

auto SerializationContext::serializerTag() const noexcept -> const std::type_info&
{
    return m_serializer_type;
}

} // namespace isml
