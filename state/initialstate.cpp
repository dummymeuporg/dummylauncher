#include <QDebug>
#include <QString>

#include "initialstate.h"

SessionState::InitialState::InitialState(::Dialog& dialog)
    : SessionState::State(dialog)
{

}

SessionState::InitialState::~InitialState()
{

}


void SessionState::InitialState::onRead(QByteArray& buf)
{
    qDebug() << "Initial state: " << buf.data();
}
