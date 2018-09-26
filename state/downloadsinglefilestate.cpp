#include <QDebug>

#include "dialog.h"

#include "state/downloadfilesstate.h"
#include "state/downloadsinglefilestate.h"

SessionState::DownloadSingleFileState::DownloadSingleFileState(
    ::Dialog& dialog, QQueue<QString>::const_iterator currentFile,
    quint32 fileSize) : SessionState::State(dialog),
                        m_currentFile(currentFile),
                        m_fileSize(fileSize),
                        m_bytesWritten(0),
                        m_fstream(*m_currentFile)
{
    m_fstream.open(QIODevice::WriteOnly);
}

SessionState::DownloadSingleFileState::~DownloadSingleFileState()
{
    m_fstream.close();
}

void SessionState::DownloadSingleFileState::onRead(QByteArray& buf)
{
    qDebug() << "Read " << buf.size() << " bytes";
    m_fstream.write(buf);
    m_fstream.flush();
    m_bytesWritten += quint32(buf.size());
    qDebug() << "Bytes written: " << m_bytesWritten;

    if (m_bytesWritten == m_fileSize)
    {
        qDebug() << "All bytes written for " << *m_currentFile;
        m_dialog.updateDownloadProgress();

        ++m_currentFile;
        qDebug() << "Fooaaa.";
        if (m_currentFile != m_dialog.downloadList().end())
        {
            qDebug() << "Get to next file:";

            m_dialog.setState(
                new SessionState::DownloadFilesState(m_dialog, m_currentFile));
        }
        else
        {
            qDebug() << "End.";
        }
    }

}
