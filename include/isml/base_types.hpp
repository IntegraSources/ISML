/**
 * @file    base_types.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    21.01.2020
 */

#ifndef ISML_BASE_TYPES_HPP
#define ISML_BASE_TYPES_HPP

#include <cstdint>
#include <cstddef>
#include <unordered_set>

namespace isml {

using Index           = std::uint8_t;              ///< Used as a sequence number for the station components.
using Indexes         = std::unordered_set<Index>; ///< Index set type.
using byte            = std::uint8_t;              ///< Byte type.
using word            = std::uint16_t;             ///< Word type.

using MessageType = std::uint16_t;
using MessageLength = std::uint16_t; ///< Message length type.
using MessageId = std::uint32_t;     ///< Message identifier type.

constexpr auto k_bad_msg_id = static_cast<MessageId>(0);

using SessionId = std::uint64_t;
const SessionId kBadSessionId = 0;

} // namespace isml

#endif // ISML_BASE_TYPES_HPP
