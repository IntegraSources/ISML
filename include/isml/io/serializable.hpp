/**
 * @file    serializable.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    21.01.2020
 */

#ifndef ISML_SERIALIZABLE_HPP
#define ISML_SERIALIZABLE_HPP

#include <cstddef>
#include <istream>
#include <ostream>

namespace isml {

/**
 * @class  Serializable
 * @brief  Provides interface that allows an objects to control
 *         its own serialization.
 */

class Serializable
{
protected:
    ~Serializable() = default;

public:

    /**
     * @brief   Writes object state into output stream.
     *
     * @param   os  Output stream.
     *
     * @return  If success - returns number of written bytes,
     *          otherwise - exception should be thrown.
     */

    virtual auto serialize(std::ostream& os) const -> std::size_t = 0;

    /**
     * @brief   Reads object state from input stream.
     *
     * @param   is  Input stream.
     *
     * @return  If success - returns number of read bytes,
     *          otherwise - exception should be thrown.
     */

    virtual auto deserialize(std::istream& is) -> std::size_t = 0;

    /**
     * @brief   Calculates size of serialized data.
     *
     * @return  Number of bytes required to store an serialized object state.
     */

    virtual auto serializedSize() const noexcept -> std::size_t = 0;
};

} // namespace isml

#endif // ISML_SERIALIZABLE_HPP
