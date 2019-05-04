#include <QDebug>

#include "protocol/updaterprotocol.h"

#include "state/updater_state/updaterdownloadfilesstate.h"
#include "state/updater_state/updaterdownloadsinglefilestate.h"

namespace SessionState
{
UpdaterDownloadSingleFileState::UpdaterDownloadSingleFileState(
    ::Protocol& protocol, QQueue<QString>::const_iterator currentFile,
    quint32 fileSize) : State(protocol),
                        m_currentFile(currentFile),
                        m_fileSize(fileSize),
                        m_bytesWritten(0),
                        m_fstream(*m_currentFile)
{
    qDebug() << "Current file: " << *m_currentFile;
    if (!m_fstream.open(QIODevice::WriteOnly))
    {
        qDebug() << "Couln not open file!";
    }
}

UpdaterDownloadSingleFileState::~UpdaterDownloadSingleFileState()
{
    m_fstream.close();
}

void UpdaterDownloadSingleFileState::onRead(QByteArray& buf)
{
    // I think this is kinda ugly.
    UpdaterProtocol* protocol = static_cast<UpdaterProtocol*>(&m_protocol);
    qDebug() << "Read " << buf.size() << " bytes";
    m_fstream.write(buf);
    m_fstream.flush();
    m_bytesWritten += quint32(buf.size());
    qDebug() << "Bytes written: " << m_bytesWritten;

    if (m_bytesWritten == m_fileSize)
    {
        qDebug() << "All bytes written for " << *m_currentFile;
        //m_dialog.updateDownloadProgress();

        ++m_currentFile;

        if (m_currentFile != protocol->downloadList().end())
        {
            qDebug() << "Get to next file:";

            m_protocol.setState(
                new UpdaterDownloadFilesState(m_protocol, m_currentFile));
        }
        else
        {
            qDebug() << "End.";
        }
    }

}

} // namespace SessionState
