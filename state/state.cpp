#include "protocol/protocol.h"
#include "state/state.h"

SessionState::State::State(::Protocol& protocol)
    : m_protocol(protocol)
{

}

SessionState::State::~State()
{

}
