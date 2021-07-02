/**
 * @file    stream_utils.cpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/24/20
 */

#include <isml/utility/stream_utils.hpp>

#include <algorithm>
#include <iterator>

namespace isml {

auto StreamUtils::copy(std::istream& stream, char* dst) -> std::size_t
{
    const auto pos = stream.tellg();
    const auto ptr =
        std::copy(std::istreambuf_iterator<char>(stream.rdbuf()),
                  std::istreambuf_iterator<char>(),
                  dst);

    stream.clear();
    stream.seekg(pos);

    return static_cast<std::size_t>(ptr - dst);
}

} // namespace isml
