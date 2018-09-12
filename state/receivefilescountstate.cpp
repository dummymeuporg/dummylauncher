#include "dialog.h"

#include "state/receivefilescountstate.h"
#include "state/receivefilesstate.h"

SessionState::ReceiveFilesCountState::ReceiveFilesCountState(::Dialog& dialog)
    : SessionState::State(dialog)
{

}

SessionState::ReceiveFilesCountState::~ReceiveFilesCountState()
{

}

void SessionState::ReceiveFilesCountState::onRead(QByteArray& buf)
{
    QDataStream in(buf);
    in.setByteOrder(QDataStream::LittleEndian);
    quint32 filecount;

    in >> filecount;
    m_dialog.setStatus(tr("Number of files: %n", "", int(filecount)));
    m_dialog.setState(
        new SessionState::ReceiveFilesState(m_dialog, filecount));
}
