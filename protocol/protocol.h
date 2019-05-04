#pragma once

#include <QObject>
#include <QString>
#include <QtNetwork>

#include "state/state.h"

class Protocol : public QObject
{
    Q_OBJECT
public:
    Protocol();
    virtual ~Protocol();
    void connect(const QString&, unsigned short);
    void setState(SessionState::State*);
    QTcpSocket* socket() {
        return m_socket;
    }
signals:
    void payloadDecoded();
public slots:
    virtual void onConnect() = 0;
    virtual void onDisconnect() = 0;
private slots:
    void _onDataReceived();
    void _processData();
    void _onSocketError(QAbstractSocket::SocketError error);
private:

    quint16 m_payloadSize;
    QQueue<QByteArray> m_decodedChunks;
protected:
    QTcpSocket* m_socket;
    std::unique_ptr<SessionState::State> m_state;
};
