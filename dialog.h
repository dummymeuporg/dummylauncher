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

    const QQueue<QString>& downloadList() const
    {
        return m_downloadList;
    }

    void updateDownloadProgress();

signals:
    void packetStored();
    void payloadDecoded();
private slots:
    void onDataReceived();
    void onConnect();
    void onDisconnect();
    void onSocketError(QAbstractSocket::SocketError);
    void processData();

private:
    Ui::Dialog *ui;
    QTcpSocket *m_socket;
    std::unique_ptr<SessionState::State> m_state;
    quint16 m_payloadSize;
    QByteArray m_payload;
    QQueue<QString> m_downloadList;
    QQueue<QByteArray> m_decodedChunks;
    bool m_readPayloadSize;
    int m_downloadedFiles;

};
