#include <QByteArray>
#include <QCryptographicHash>
#include <QString>

#include "dialog.h"

#include "state/masterinitialstate.h"

SessionState::MasterInitialState::MasterInitialState(::Dialog& dialog)
    : SessionState::State(dialog)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    QString login(dialog.login());
    QString password(dialog.password());


    QCryptographicHash hasher(QCryptographicHash::Sha3_512);
    hasher.addData(QByteArray::fromRawData(login.toStdString().c_str(),
                                           login.size()));
    hasher.addData(QByteArray::fromRawData(password.toStdString().c_str(),
                                           password.size()));

    QByteArray hashedPassword(hasher.result());
    quint16 size = quint16(login.size()) + quint16(hashedPassword.size());
    out << size;

    packet.append(login.toStdString().data(), login.size());
    packet.append(hashedPassword);
    m_dialog.masterSocket().write(packet);
}

SessionState::MasterInitialState::~MasterInitialState()
{

}

void SessionState::MasterInitialState::onRead(QByteArray& buf)
{
    return;
}
