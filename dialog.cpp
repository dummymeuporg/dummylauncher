#include <QDebug>

#include "state/state.h"

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_socket(nullptr)
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
    ui->labelUpdate->setText(tr("Connecting to update server..."));
    m_socket->connectToHost("kojiro.gcir.ovh", 8087);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onDataReceived()
{
    qDebug() << m_socket->bytesAvailable();
}

void Dialog::onConnect()
{
    ui->labelUpdate->setText(tr("Connected! Computing differences..."));
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    quint16 version = 1;
    out << sizeof(quint16) << version;
    m_socket->write(paquet);
}

void Dialog::onDisconnect()
{

}

void Dialog::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
}
