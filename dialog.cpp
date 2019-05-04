#include <QDebug>

#include "protocol/updaterprotocol.h"

#include "state/updater_state/updaterinitalstate.h"
#include "state/masterinitialstate.h"


#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    m_socket(nullptr),
    m_masterSocket(nullptr),
    m_masterState(nullptr),
    m_updaterProtocol(*this),
    m_payloadSize(0),
    m_readPayloadSize(true),
    m_downloadedFiles(0)
{
    ui->setupUi(this);

    /*
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
    QString hostname = "localhost";
    m_socket->connectToHost(hostname, 8087);
    */
    setStatus(tr("Connecting to update server..."));
    m_updaterProtocol.setState(
        new SessionState::UpdaterInitialState(m_updaterProtocol)
    );
    m_updaterProtocol.connect("localhost", 8087);
}

Dialog::~Dialog()
{
    delete ui;
    delete m_socket;
    delete m_masterSocket;
}


void Dialog::setMasterState(SessionState::State* state)
{
    m_masterState.reset(state);
}

void Dialog::setStatus(const QString& status)
{
    ui->labelUpdate->setText(status);
}

void Dialog::onPushButtonConnectClick()
{
    setStatus(tr("Connecting to master server..."));
    /*
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
    QString hostname = "127.0.0.1";
    m_masterSocket->connectToHost(hostname, 33337);
    */

}


void Dialog::updateDownloadProgress()
{

    /*
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
    */
}

void Dialog::onMasterConnect()
{
    setStatus(tr("Connected! Authenticating..."));
    //setMasterState(new SessionState::MasterInitialState(*this));

}

void Dialog::onMasterDataReceived()
{

}

void Dialog::onMasterDisconnect()
{
    setStatus(tr("Connection with the master server lost!"));
}

void Dialog::onMasterSocketError(QAbstractSocket::SocketError)
{
    setStatus(tr("Could not reach master server!"));
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
