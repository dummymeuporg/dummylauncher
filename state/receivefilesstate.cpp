#include <QDebug>

#include "state/receivefilesstate.h"

SessionState::ReceiveFilesState::ReceiveFilesState(::Dialog& dialog)
    : SessionState::State(dialog)
{

}

SessionState::ReceiveFilesState::~ReceiveFilesState()
{

}

void SessionState::ReceiveFilesState::onRead(QByteArray& buf)
{
    qDebug() << "ReceiveFilesState: " << buf.data();

    // Separate hash from filename
    QString filename(buf.left(buf.length() - 20));

    qDebug() << "Filename: " << filename;

}
