#ifndef CTCPCOM_H
#define CTCPCOM_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include "public.h"
#include "cproccookeddata.h"

class cTCPCom : public QObject
{
    Q_OBJECT

public:
    cTCPCom(void);
    ~cTCPCom(void);

private:
    THREAD_STATUS m_threadStatus;

    QTcpSocket *m_tcpSocket = nullptr;
    QHostAddress m_hostAddr;
    int m_hostPort;

    QByteArray m_dataToWrite;
    QMutex m_mutex_threadStaus;
    QMutex m_mutex_dataToWrite;

    ERROR_CODE (*m_handleDataFun)(QByteArray, cProcCookedData*);
    cProcCookedData* m_procDataDevice;

private:
    ERROR_CODE connectToServer(void);
    QByteArray readData(void);
    ERROR_CODE writeData(QByteArray buffer);

    void setThreadStatus(THREAD_STATUS &status);
    THREAD_STATUS getThreadStatus(void);

public:
    bool isConnected(void);

    ERROR_CODE initialize(QString serverIP, int serverPort);
    ERROR_CODE pauseConnection(void);
    ERROR_CODE continueConnection(void);
    ERROR_CODE stopConnection(void);

    static ERROR_CODE setDataToSend(TAG_ANCHOR_DATA &data, cTCPCom* thisTcp);

    void setHandleDataFun(ERROR_CODE (*handleDataFun)(QByteArray, cProcCookedData*), cProcCookedData* device);

public slots:
    void doWorks(void);

signals:
    void workReady(void);
};

#endif // CTCPCOM_H
