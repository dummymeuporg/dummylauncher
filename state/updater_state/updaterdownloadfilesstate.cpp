#include <QDebug>
#include <QString>

#include "protocol/protocol.h"

#include "state/updater_state/updaterdownloadfilesstate.h"
#include "state/updater_state/updaterdownloadsinglefilestate.h"


namespace SessionState
{

UpdaterDownloadFilesState::UpdaterDownloadFilesState(
    ::Protocol& protocol, QQueue<QString>::const_iterator iterator)
    : State(protocol), m_fileIterator(iterator)
{
    _requestFile(*m_fileIterator);
}

UpdaterDownloadFilesState::~UpdaterDownloadFilesState()
{

}

void UpdaterDownloadFilesState::_requestFile(const QString& filename)
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
    m_protocol.socket()->write(packet);
}

void UpdaterDownloadFilesState::onRead(QByteArray& buf)
{
    QDataStream in(buf);
    in.setByteOrder(QDataStream::LittleEndian);
    quint16 code;
    quint32 filesize;

    in >> code >> filesize;
    qDebug() << "Code:" << code;
    qDebug() << "File size: " << filesize;
    m_protocol.setState(
        new UpdaterDownloadSingleFileState(
            m_protocol,
            m_fileIterator,
            filesize
        )
    );
}

} // namespace SessionState
