#include "dialog.h"

#include "protocol/updaterprotocol.h"

UpdaterProtocol::UpdaterProtocol(Dialog& dialog)
    : m_dialog(dialog)
{

}

void UpdaterProtocol::onConnect() {
    qDebug() << "UpdaterProtocol: connected.";
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    quint16 version = 1;
    out << quint16(sizeof(quint16)) << version;
    m_socket->write(paquet);
}

void UpdaterProtocol::onDisconnect() {

}

void UpdaterProtocol::addDownload(const QString& filename) {
    m_downloadList.enqueue(filename);
}

void UpdaterProtocol::updateDownloadProgress()
{
    qDebug() << "TODO: update download progress.";
}
