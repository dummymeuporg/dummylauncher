#include <QDebug>
#include <QDir>
#include <QFileInfo>

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

    QFileInfo fileInfo(filename);
    QDir dir(fileInfo.dir());
    if (!dir.exists())
    {
        // Create dir.
        dir.mkdir(".");

        // Add file to download.
        return;
    }

    if (!fileInfo.exists())
    {
        // Add file to download.
        return;
    }

    // Compute sha1 & compare.


}
