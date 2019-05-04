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


}


void Dialog::updateDownloadProgress()
{

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
