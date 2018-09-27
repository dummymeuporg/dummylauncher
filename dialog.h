#pragma once

#include <memory>

#include <QDialog>
#include <QQueue>
#include <QString>
#include <QtNetwork>

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

    void setState(SessionState::State* state);

    void addDownload(const QString&);

    void setStatus(const QString&);

    QTcpSocket& socket() {
        return *m_socket;
    }

    QTcpSocket& masterSocket()
    {
        return *m_masterSocket;
    }

    const QQueue<QString>& downloadList() const
    {
        return m_downloadList;
    }

    void updateDownloadProgress();

    QString login() const;
    QString password() const;

signals:
    void packetStored();
    void payloadDecoded();
    void payloadMasterDecoded();
private slots:
    void onDataReceived();
    void onConnect();
    void onDisconnect();
    void onSocketError(QAbstractSocket::SocketError);
    void processData();

    void onMasterDataReceived();
    void onMasterConnect();
    void onMasterDisconnect();
    void onMasterSocketError(QAbstractSocket::SocketError);
    void processMasterData();

    void onPushButtonConnectClick();

private:
    Ui::Dialog *ui;
    QTcpSocket *m_socket, *m_masterSocket;
    std::unique_ptr<SessionState::State> m_state;
    quint16 m_payloadSize;
    QByteArray m_payload;
    QQueue<QString> m_downloadList;
    QQueue<QByteArray> m_decodedChunks;
    bool m_readPayloadSize;
    int m_downloadedFiles;

};
