#ifndef CTCPCOM_H
#define CTCPCOM_H

#include <QTcpSocket>
#include <QDataStream>
#include <QHostAddress>
#include <QByteArray>
#include <QMutex>
#include "public.h"
#include "cprocrawdata.h"

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
    QHostAddress m_hostAddr;
    int m_hostPort;

    QByteArray m_dataToWrite;
    QMutex m_mutex_threadStaus;
    QMutex m_mutex_PrintStatus;
    QMutex m_mutex_dataToWrite;

    ERROR_CODE (*m_handleDataFun)(QByteArray, cProcRawData*);
    cProcRawData* m_procDataDevice;

private:
    ERROR_CODE connectToServer(void);
    QByteArray readData(void);
    ERROR_CODE writeData(QByteArray buffer);

    void setThreadStatus(THREAD_STATUS &status);
    THREAD_STATUS getThreadStatus(void);
    bool getPrintStatus(void);

public:
    bool isConnected(void);

    void switchPrintOnOff(void);

    ERROR_CODE initialize(QString serverIP, int serverPort);
    ERROR_CODE pauseConnection(void);
    ERROR_CODE continueConnection(void);
    ERROR_CODE stopConnection(void);

    static ERROR_CODE setDataToSend(TAG_ANCHOR_DATA& data, cTCPCom* thisTcp);

    void setHandleDataFun(ERROR_CODE (*handleDataFun)(QByteArray, cProcRawData*), cProcRawData* device);

public slots:
    void doWorks(void);

signals:
    void workReady(void);
};

#endif // CTCPCOM_H
