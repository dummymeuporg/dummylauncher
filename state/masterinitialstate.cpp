#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QString>

#include "protocol/protocol.h"

#include "state/masterinitialstate.h"

namespace SessionState
{
MasterInitialState::MasterInitialState(::Protocol& protocol) : State(protocol)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    /* FIXME.
    QString login(dialog.login().toUpper());
    QString password(dialog.password());


    QCryptographicHash hasher(QCryptographicHash::Sha512);
    hasher.addData(QByteArray::fromRawData(login.toStdString().c_str(),
                                           login.size()));
    hasher.addData(QByteArray::fromRawData(password.toStdString().c_str(),
                                           password.size()));
    QByteArray hashedPassword(hasher.result());
    qDebug() << hashedPassword;
    quint16 size = quint16(login.size()) + quint16(hashedPassword.size());
    out << size;

    packet.append(login.toStdString().data(), login.size());
    packet.append(hashedPassword);
    */
    m_protocol.socket()->write(packet);
}

SessionState::MasterInitialState::~MasterInitialState()
{

}

void SessionState::MasterInitialState::onRead(QByteArray& buf)
{
    return;
}

} // namespace SessionState
