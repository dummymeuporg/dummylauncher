#include <QString>

#include "dialog.h"

#include "state/downloadfilesstate.h"

SessionState::DownloadFilesState::DownloadFilesState(::Dialog& dialog)
    : SessionState::State(dialog),
      m_fileIterator(m_dialog.downloadList().begin())
{

}

void SessionState::DownloadFilesState::_requestFile(const QString& filename)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    quint16 command = CODE_SEND;
    quint16 size = quint16(sizeof(command)) + quint16(filename.size());

    out << size << command << filename.toLatin1();

    m_dialog.socket().write(packet);
}
