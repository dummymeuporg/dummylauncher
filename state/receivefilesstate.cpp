#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QString>

#include "dialog.h"

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
    QByteArray hash(buf.right(20));

    qDebug() << "Filename: " << filename;
    qDebug() << "Sum: " << hash;

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
        m_dialog.addDownload(filename);
        return;
    }

    // Compute sha1 & compare.
    QByteArray computedHash(filesum(filename));

    if (computedHash != hash)
    {
        qDebug() << "Hashes don't match. Download.";
        m_dialog.addDownload(filename);
    }
    else
    {
        qDebug() << "Hashes match.";
    }


}


QByteArray
SessionState::ReceiveFilesState::filesum(const QString& filename)
{
    QFile file(filename);
    QCryptographicHash hasher(QCryptographicHash::Sha1);
    QByteArray chunk(SessionState::ReceiveFilesState::CHUNK_SIZE, '\0');
    qint64 count = 0;
    file.open(QIODevice::ReadOnly);

    do
    {
        count = file.read(chunk.data(),
                          SessionState::ReceiveFilesState::CHUNK_SIZE);
        hasher.addData(chunk);
    } while (count == SessionState::ReceiveFilesState::CHUNK_SIZE);

    return hasher.result();

}
