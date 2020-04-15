#ifndef CTCPCOM_H
#define CTCPCOM_H

#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QByteArray>

class cTCPCom : public QObject
{
    Q_OBJECT

public:
    cTCPCom();
    ~cTCPCom();
private:
    QTcpSocket *m_tcpSocket = nullptr;
    QDataStream m_in, m_out;
    QHostAddress m_hostAddr;
    int m_hostPort;

public:
    void writeData(QByteArray buffer);
private slots:
    void readData(void);
};

#endif // CTCPCOM_H
