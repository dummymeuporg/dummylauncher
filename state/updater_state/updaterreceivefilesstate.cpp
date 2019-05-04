#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QString>

#include <exception>

#include "protocol/updaterprotocol.h"

#include "state/updater_state/updaterdownloadfilesstate.h"
#include "state/updater_state/updaterreceivefilesstate.h"


namespace SessionState
{
struct NullFileCountException : public std::exception
{
    const char* what() const noexcept
    {
        return "File count cannot be empty.";
    }
};

UpdaterReceiveFilesState::UpdaterReceiveFilesState(::Protocol& protocol,
                                                   quint32 filecount)
    : State(protocol), m_filecount(filecount),
      m_currentFile(0)
{
    if (m_filecount == 0)
    {
        throw NullFileCountException();
    }
}

UpdaterReceiveFilesState::~UpdaterReceiveFilesState()
{

}

void UpdaterReceiveFilesState::onRead(QByteArray& buf)
{
    // I think this is kinda ugly.
    UpdaterProtocol* protocol = static_cast<UpdaterProtocol*>(&m_protocol);
    qDebug() << "ReceiveFilesState: " << buf.data();

    // Separate hash from filename
    QString filename(buf.left(buf.length() - 20));
    QByteArray hash(buf.right(20));

    qDebug() << "Filename: " << filename;
    qDebug() << "Sum: " << hash;

    QFileInfo fileInfo(filename);
    QDir dir(fileInfo.dir());
    qDebug() << "Absolute path: " << dir.absolutePath();
    if (!dir.exists())
    {
        // Create dir.
        if (!QDir().mkdir(fileInfo.dir().absolutePath()))
        {
            qDebug() << "Could not create directory " << dir << "!";
        }

        qDebug() << "Dir does not exist. Add " << filename << " to download.";

        // Add file to download.
        protocol->addDownload(filename);
    } else if (!fileInfo.exists())
    {
        // Add file to download.
        qDebug() << "File does not exist. Add " << filename << " to download.";
        protocol->addDownload(filename);
    } else {
        // Compute sha1 & compare.
        QByteArray computedHash(filesum(filename));
        qDebug() << "Computed sum: " << computedHash;
        if (!_hashesMatch(computedHash, hash))
        {
            qDebug() << "Hashes don't match. Download.";
            protocol->addDownload(filename);
        }
        else
        {
            qDebug() << "Hashes match.";
        }
    }

    if (++m_currentFile == m_filecount)
    {
        if (protocol->downloadList().begin() != protocol->downloadList().end())
        {
            qDebug() << "Switch to DownloadFilesState";
            m_protocol.setState(
                new UpdaterDownloadFilesState(
                    m_protocol, protocol->downloadList().begin()));
        }
        else
        {
            qDebug() << "No file to download.";
            protocol->updateDownloadProgress();
        }
    }
}

bool UpdaterReceiveFilesState::_hashesMatch(const QByteArray& h1,
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
UpdaterReceiveFilesState::filesum(const QString& filename)
{
    QFile file(filename);
    QCryptographicHash hasher(QCryptographicHash::Sha1);
    QByteArray chunk(CHUNK_SIZE, '\0');
    qint64 count = 0;
    file.open(QIODevice::ReadOnly);

    do
    {
        count = file.read(chunk.data(), CHUNK_SIZE);
        hasher.addData(chunk, qint32(count));
    } while (count == CHUNK_SIZE);

    return hasher.result();

}

} // UpdaterReceiveFilesState
