#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QString>

#include <exception>

#include "dialog.h"

#include "state/downloadfilesstate.h"
#include "state/receivefilesstate.h"

struct NullFileCountException : public std::exception
{
    const char* what() const noexcept
    {
        return "File count cannot be empty.";
    }
};

SessionState::ReceiveFilesState::ReceiveFilesState(::Dialog& dialog,
                                                   quint32 filecount)
    : SessionState::State(dialog), m_filecount(filecount),
      m_currentFile(0)
{
    if (m_filecount == 0)
    {
        throw NullFileCountException();
    }
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

        qDebug() << "Dir does not exist. Add " << filename << " to download.";

        // Add file to download.
        m_dialog.addDownload(filename);
    } else if (!fileInfo.exists())
    {
        // Add file to download.
        qDebug() << "File does not exist. Add " << filename << " to download.";
        m_dialog.addDownload(filename);
    } else {
        // Compute sha1 & compare.
        QByteArray computedHash(filesum(filename));
        qDebug() << "Computed sum: " << computedHash;
        if (!_hashesMatch(computedHash, hash))
        {
            qDebug() << "Hashes don't match. Download.";
            m_dialog.addDownload(filename);
        }
        else
        {
            qDebug() << "Hashes match.";
        }
    }

    if (++m_currentFile == m_filecount)
    {
        if (m_dialog.downloadList().begin() != m_dialog.downloadList().end())
        {
            qDebug() << "Switch to DownloadFilesState";
            m_dialog.setState(
                new SessionState::DownloadFilesState(
                    m_dialog, m_dialog.downloadList().begin()));
        }
        else
        {
            qDebug() << "No file to download.";
            m_dialog.updateDownloadProgress();
        }
    }
}

bool SessionState::ReceiveFilesState::_hashesMatch(const QByteArray& h1,
                                                   const QByteArray& h2)
{
    if (h1.size() != h2.size())
    {
        return false;
    }

    for (int i = 0; i < h1.size(); i++)
    {
        if (h1[i] != h2[i])
        {
            qDebug() << quint8(h1[i]) << quint8(h2[i]);
            return false;
        }
    }

    return true;
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
        hasher.addData(chunk, qint32(count));
    } while (count == SessionState::ReceiveFilesState::CHUNK_SIZE);

    return hasher.result();

}
