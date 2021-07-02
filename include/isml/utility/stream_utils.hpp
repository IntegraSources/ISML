/**
 * @file    stream_utils.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    11/24/20
 */

#ifndef ISML_STREAM_UTILS_HPP
#define ISML_STREAM_UTILS_HPP

#include <istream>
#include <iterator>
#include <algorithm>

#include <isml/base_types.hpp>

namespace isml {

class StreamUtils final
{
public:
    using ByteIterator = std::istreambuf_iterator<char>;

public:
    static auto copy(std::istream& stream, char* dst) -> std::size_t;

    template<typename F, typename Iterator = ByteIterator>
    static auto foreach(std::istream& stream, F&& action) -> void;
};

template<typename F, typename Iterator>
auto StreamUtils::foreach (std::istream& stream, F && action) -> void
{
    const auto pos = stream.tellg();
    std::for_each(Iterator(stream), Iterator(), std::forward<F>(action));
    stream.clear();
    stream.seekg(pos);
}

} // namespace isml

#endif // ISML_STREAM_UTILS_HPP
