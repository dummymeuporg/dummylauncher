#pragma once

#include <memory>

#include <QDialog>
#include <QQueue>
#include <QString>
#include <QtNetwork>

#include "protocol/updaterprotocol.h"

namespace SessionState
{
    class State;
}

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void setMasterState(SessionState::State* state);

    void setStatus(const QString&);

    QTcpSocket& socket() {
        return *m_socket;
    }

    QTcpSocket& masterSocket()
    {
        return *m_masterSocket;
    }

    void updateDownloadProgress();

    QString login() const;
    QString password() const;

signals:
    void packetStored();
    void payloadDecoded();
    void payloadMasterDecoded();
private slots:
    void onMasterDataReceived();
    void onMasterConnect();
    void onMasterDisconnect();
    void onMasterSocketError(QAbstractSocket::SocketError);
    void processMasterData();

    void onPushButtonConnectClick();

private:
    Ui::Dialog *ui;
    QTcpSocket *m_socket, *m_masterSocket;
    std::unique_ptr<SessionState::State> m_masterState;
    UpdaterProtocol m_updaterProtocol;
    quint16 m_payloadSize;
    QByteArray m_payload;
    QQueue<QByteArray> m_decodedChunks;
    bool m_readPayloadSize;
    int m_downloadedFiles;

};
