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
                     SLOT(processPayload()));
}

Protocol::~Protocol()
{
    delete m_socket;
}

void Protocol::_onDataReceived()
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

void Protocol::_processData()
{
    QByteArray payload = m_decodedChunks.dequeue();
    m_state->onRead(m_payload);
}

void Protocol::_onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
}
