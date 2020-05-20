#ifndef CTCPCOM_H
#define CTCPCOM_H

#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QByteArray>
#include <QMutex>
#include "public.h"

class cTCPCom : public QObject
{
    Q_OBJECT

public:
    cTCPCom();
    ~cTCPCom();
private:
    bool m_isPrintingInfo;
    THREAD_STATUS m_threadStatus;

    QTcpSocket *m_tcpSocket = nullptr;
    QDataStream m_in, m_out;
    QHostAddress m_hostAddr;
    int m_hostPort;

    QByteArray m_dataToWrite;
    QMutex m_mutex_threadStaus;
    QMutex m_mutex_PrintStatus;
    QMutex m_mutex_dataToWrite;

    ERROR_CODE (*m_handleDataFun)(QByteArray data);

private:
    ERROR_CODE connectToServer(void);
    QByteArray readData(void);
    void writeData(QByteArray buffer);
    void setThreadStatus(THREAD_STATUS &status);

public:
    bool isConnected(void);

    void switchPrintOnOff(void);

    ERROR_CODE initialize(QString serverIP, int serverPort);
    ERROR_CODE pauseConnection(void);
    ERROR_CODE continueConnection(void);
    ERROR_CODE stopConnection(void);

    void setDataToSend(QByteArray data);

    void setHandleDataFun(ERROR_CODE (*handleDataFun)(QByteArray data));

public slots:
    void doWorks(void);

signals:
    void workReady(void);
};

#endif // CTCPCOM_H
