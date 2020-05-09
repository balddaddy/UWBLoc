#include <iostream>
#include "ctcpcom.h"

cTCPCom::cTCPCom()
    :m_tcpSocket(new QTcpSocket(this))
{
    m_in.setDevice(m_tcpSocket);
    m_in.setVersion(QDataStream::Qt_5_0);
    m_out.setDevice(m_tcpSocket);
    m_out.setVersion(QDataStream::Qt_5_0);
}

cTCPCom::~cTCPCom()
{
    delete m_tcpSocket;
}

void cTCPCom::writeData(QByteArray buffer)
{
    m_tcpSocket->write(buffer.data(),buffer.size());
    std::cout << "Send data:\t" << buffer.data() << std::endl;
}

void cTCPCom::readData(void)
{
    QByteArray buffer;
    m_tcpSocket->waitForReadyRead(300);
    buffer = m_tcpSocket->readAll();
    std::cout << "Receive data:\t" << buffer.data() << std::endl;
}

bool cTCPCom::isConnected(void)
{
    m_hostAddr = QHostAddress::LocalHost;
    m_hostPort = 6666;
    m_tcpSocket->connectToHost(m_hostAddr,m_hostPort);
    bool succ = m_tcpSocket->waitForConnected(3000);
    if (succ)
        QObject::connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    return succ;
}
