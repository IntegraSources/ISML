/**
 * @file    fake_session.hpp
 * @author  Oleg E. Vorobiov <o.vorobiov(at)integrasources.com>
 * @date    19.04.2020
 */

#ifndef ISML_FAKE_SESSION_HPP
#define ISML_FAKE_SESSION_HPP

#include <memory>

#include <isml/session/session.hpp>

#include <isml/transport/fake_transport.hpp>

namespace isml {

class FakeSession : public Session
{
public:
    FakeSession()
        : Session(0xDEADBEAF, std::make_unique<FakeTransport>())
    {}
};

} // namespace isml

#endif // ISML_FAKE_SESSION_HPP
