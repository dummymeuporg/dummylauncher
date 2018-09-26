#include <QDebug>
#include <QString>

#include "dialog.h"

#include "state/downloadfilesstate.h"
#include "state/downloadsinglefilestate.h"

SessionState::DownloadFilesState::DownloadFilesState(
    ::Dialog& dialog, QQueue<QString>::const_iterator iterator)
    : SessionState::State(dialog),
      m_fileIterator(iterator)
{
    _requestFile(*m_fileIterator);
}

SessionState::DownloadFilesState::~DownloadFilesState()
{

}

void SessionState::DownloadFilesState::_requestFile(const QString& filename)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    quint16 command = CODE_SEND;
    quint16 size = quint16(sizeof(command)) + quint16(filename.size());

    out << size << command;
    packet.append(filename.toStdString().data(), filename.size());

    qDebug() << "Filename: " << filename;
    qDebug() << "Filename size: " << filename.size();
    qDebug() << "Size: " << packet.size();
    m_dialog.socket().write(packet);
}

void SessionState::DownloadFilesState::onRead(QByteArray& buf)
{
    QDataStream in(buf);
    in.setByteOrder(QDataStream::LittleEndian);
    quint16 code;
    quint32 filesize;

    in >> code >> filesize;
    qDebug() << "Code:" << code;
    qDebug() << "File size: " << filesize;
    m_dialog.setState(
        new SessionState::DownloadSingleFileState(m_dialog,
                                                  m_fileIterator,
                                                  filesize));
}
