#include <iostream>
#include "ctcpcom.h"

cTCPCom::cTCPCom()
    :m_tcpSocket(new QTcpSocket(this))
{
    m_in.setDevice(m_tcpSocket);
    m_in.setVersion(QDataStream::Qt_5_0);
    m_out.setDevice(m_tcpSocket);
    m_out.setVersion(QDataStream::Qt_5_0);
    QObject::connect(m_tcpSocket, &QTcpSocket::readyRead, this, &cTCPCom::readData);

    m_hostAddr = QHostAddress("10.21.11.66");
    m_hostPort = 666;
    m_tcpSocket->connectToHost(m_hostAddr,m_hostPort);
}

cTCPCom::~cTCPCom()
{
    delete m_tcpSocket;
}

void cTCPCom::writeData(QByteArray buffer)
{
    m_tcpSocket->write(buffer.data());
    std::cout << "Send data:\t" << buffer.data() << std::endl;
}

void cTCPCom::readData(void)
{
    QByteArray buffer;
    buffer = m_tcpSocket->readAll();
    std::cout << "Receive data:\t" << buffer.data() << std::endl;
}
