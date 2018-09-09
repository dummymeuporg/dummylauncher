#include <QDebug>
#include <QString>

#include "dialog.h"

#include "state/initialstate.h"
#include "state/receivefilesstate.h"

SessionState::InitialState::InitialState(::Dialog& dialog)
    : SessionState::State(dialog)
{

}

SessionState::InitialState::~InitialState()
{

}


void SessionState::InitialState::onRead(QByteArray& buf)
{
    QString data(buf);
    qDebug() << "Initial state: " << buf.data();
    if (data == "OK")
    {
        qDebug() << "Switch to receive files state.";
        m_dialog.setState<SessionState::ReceiveFilesState>();
    }
}
