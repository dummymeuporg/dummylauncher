#include <QByteArray>

#include "dialog.h"

#include "state/masterinitialstate.h"

SessionState::MasterInitialState::MasterInitialState(::Dialog& dialog)
    : SessionState::State(dialog)
{

}

SessionState::MasterInitialState::~MasterInitialState()
{

}

void SessionState::MasterInitialState::onRead(QByteArray& buf)
{
    return;
}
