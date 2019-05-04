#include "protocol/protocol.h"

Protocol::Protocol() : m_socket(new QTcpSocket(this))
{
    QObject::connect(m_socket,
                     SIGNAL(readyRead()),
                     this,
                     SLOT(_onDataReceived()));
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
                     SLOT(_onSocketError(QAbstractSocket::SocketError)));
    QObject::connect(this,
                     SIGNAL(payloadDecoded()),
                     this,
                     SLOT(_processData()));
}

Protocol::~Protocol()
{
    delete m_socket;
}

void Protocol::_onDataReceived()
{
    qDebug() << "Bytes available: " <<
                m_socket->bytesAvailable();
    QByteArray payload;
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
    payload.resize(m_payloadSize);
    m_socket->read(payload.data(), m_payloadSize);
    m_decodedChunks.enqueue(payload);
    emit payloadDecoded();
    qDebug() << "Reset payload.";
    m_payloadSize = 0;

    while(m_socket->bytesAvailable() >= int(sizeof(quint16)))
    {
        qDebug() << m_socket->bytesAvailable() << "Bytes available";
        in >> m_payloadSize;
        qDebug() << "2: Set payload size to: " << m_payloadSize;
        if(m_socket->bytesAvailable() >= m_payloadSize) {
            qDebug() << "Will read " << m_payloadSize;
            payload.resize(m_payloadSize);
            m_socket->read(payload.data(), m_payloadSize);
            m_decodedChunks.enqueue(payload);
            emit payloadDecoded();
            qDebug() << "Reset payload.";
            m_payloadSize = 0;
        } else {
            break;
        }
    }
}

void Protocol::_processData()
{
    QByteArray payload = m_decodedChunks.dequeue();
    m_state->onRead(payload);
}

void Protocol::_onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
}

void Protocol::connect(const QString& host, unsigned short port) {
    m_socket->connectToHost(host, port);
}

void Protocol::setState(SessionState::State* state)
{
    m_state.reset(state);
}
