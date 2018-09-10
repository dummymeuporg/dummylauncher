#include <QDebug>

#include "state/initialstate.h"

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_socket(nullptr),
    m_state(new SessionState::InitialState(*this)),
    m_payloadSize(0)
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
    ui->labelUpdate->setText(tr("Connecting to update server..."));
    m_socket->connectToHost("kojiro.gcir.ovh", 8087);
}

Dialog::~Dialog()
{
    delete ui;
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
    }

    if (m_socket->bytesAvailable() < m_payloadSize)
    {
        qDebug() << "Too few data. " <<
                    m_socket->bytesAvailable() << "<" << m_payloadSize;
        return;
    }

    if(m_socket->bytesAvailable() > 0)
    {
        qDebug() << "Will read " << m_payloadSize;
        m_payload.resize(m_payloadSize);
        m_socket->read(m_payload.data(), m_payloadSize);
        emit payloadDecoded();
    }
    m_payloadSize = 0;
}

void Dialog::onConnect()
{
    ui->labelUpdate->setText(tr("Connected! Computing differences..."));
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    quint16 version = 1;
    out << quint16(sizeof(quint16)) << version;
    m_socket->write(paquet);
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
    m_state->onRead(m_payload);
    if (m_socket->bytesAvailable() >= int(sizeof(quint16)))
    {
        m_socket->read(reinterpret_cast<char*>(&m_payloadSize),
                       sizeof(quint16));
        qDebug() << "Next precoss: " << m_payloadSize;
        emit m_socket->readyRead();
    }
}
