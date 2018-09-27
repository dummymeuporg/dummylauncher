#include <QDebug>

#include "state/initialstate.h"

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_socket(nullptr),
    m_masterSocket(nullptr),
    m_state(new SessionState::InitialState(*this)),
    m_payloadSize(0),
    m_readPayloadSize(true),
    m_downloadedFiles(0)
{
    ui->setupUi(this);

    m_socket = new QTcpSocket(this);
    QObject::connect(m_socket,
                     SIGNAL(readyRead()),
                     this,
                     SLOT(onDataReceived()));
    QObject::connect(m_socket,
                     SIGNAL(connected()),
                     this,
                     SLOT(onConnect()));
    QObject::connect(m_socket,
                     SIGNAL(disconnected()),
                     this,
                     SLOT(onDisconnect()));
    QObject::connect(m_socket,
                     SIGNAL(error(QAbstractSocket::SocketError)),
                     this,
                     SLOT(onSocketError(QAbstractSocket::SocketError)));
    QObject::connect(this,
                     SIGNAL(payloadDecoded()),
                     this,
                     SLOT(processData()));
    setStatus(tr("Connecting to update server..."));
    QString hostname = "192.168.1.18";
    m_socket->connectToHost(hostname, 8087);
}

Dialog::~Dialog()
{
    delete ui;
    delete m_socket;
    delete m_masterSocket;
}

void Dialog::setState(SessionState::State* state)
{
    m_state.reset(state);
}

void Dialog::setStatus(const QString& status)
{
    ui->labelUpdate->setText(status);
}

void Dialog::onDataReceived()
{
    qDebug() << "Bytes available: " <<
                m_socket->bytesAvailable();
    QDataStream in(m_socket);
    in.setByteOrder(QDataStream::LittleEndian);

    if (m_payloadSize == 0)
    {
        if (m_socket->bytesAvailable() < int(sizeof(quint16)))
        {
            return;
        }
        in >> m_payloadSize;
        qDebug() << "Set payload size to: " << m_payloadSize;
    }

    if (m_socket->bytesAvailable() < m_payloadSize)
    {
        qDebug() << "Too few data. " <<
                    m_socket->bytesAvailable() << "<" << m_payloadSize;
        return;
    }

    qDebug() << "Will read " << m_payloadSize;
    m_payload.resize(m_payloadSize);
    m_socket->read(m_payload.data(), m_payloadSize);
    m_decodedChunks.enqueue(m_payload);
    emit payloadDecoded();
    qDebug() << "Reset payload.";
    m_payloadSize = 0;

    while(m_socket->bytesAvailable() >= int(sizeof(quint16)))
    {
        qDebug() << m_socket->bytesAvailable() << "Bytes available";
        in >> m_payloadSize;
        qDebug() << "2: Set payload size to: " << m_payloadSize;
        if(m_socket->bytesAvailable() >= m_payloadSize)
        {
            qDebug() << "Will read " << m_payloadSize;
            m_payload.resize(m_payloadSize);
            m_socket->read(m_payload.data(), m_payloadSize);
            m_decodedChunks.enqueue(m_payload);
            emit payloadDecoded();
            qDebug() << "Reset payload.";
            m_payloadSize = 0;
        } else
        {
            break;
        }

    }
}

void Dialog::onConnect()
{
    setStatus(tr("Connected! Computing differences..."));
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    quint16 version = 1;
    out << quint16(sizeof(quint16)) << version;
    m_socket->write(paquet);
}

void Dialog::onPushButtonConnectClick()
{
    setStatus(tr("Connecting to master server..."));
    m_masterSocket = new QTcpSocket(this);
    QObject::connect(m_masterSocket,
                     SIGNAL(readyRead()),
                     this,
                     SLOT(onMasterDataReceived()));
    QObject::connect(m_masterSocket,
                     SIGNAL(connected()),
                     this,
                     SLOT(onMasterConnect()));
    QObject::connect(m_masterSocket,
                     SIGNAL(disconnected()),
                     this,
                     SLOT(onMasterDisconnect()));
    QObject::connect(m_masterSocket,
                     SIGNAL(error(QAbstractSocket::SocketError)),
                     this,
                     SLOT(onMasterSocketError(QAbstractSocket::SocketError)));
    QObject::connect(this,
                     SIGNAL(payloadMasterDecoded()),
                     this,
                     SLOT(processMasterData()));
    QString hostname = "192.168.1.18";
    m_masterSocket->connectToHost(hostname, 33337);

    // send login + hashed password

}

void Dialog::onDisconnect()
{

}

void Dialog::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
}

void Dialog::processData()
{
    QByteArray payload = m_decodedChunks.dequeue();
    m_state->onRead(m_payload);
}

void Dialog::addDownload(const QString& filename)
{
    m_downloadList.enqueue(filename);
}

void Dialog::updateDownloadProgress()
{

    if (m_downloadedFiles == m_downloadList.size()
        || m_downloadList.size() == 0)
    {
        ui->progressBarUpdate->setValue(100);
    }
    else
    {
        ui->progressBarUpdate->setValue(
            int((float(++m_downloadedFiles)/m_downloadList.size())) * 100);
    }


    if (ui->progressBarUpdate->value() == 100)
    {
        ui->pushButtonConnect->setEnabled(true);
    }
}

void Dialog::onMasterConnect()
{
    setStatus(tr("Connected! Authenticating..."));

}

void Dialog::onMasterDataReceived()
{

}

void Dialog::onMasterDisconnect()
{

}

void Dialog::onMasterSocketError(QAbstractSocket::SocketError)
{

}

void Dialog::processMasterData()
{

}

QString Dialog::login() const
{
    return ui->lineEditLogin->text();
}

QString Dialog::password() const
{
    return ui->lineEditPassword->text();
}
