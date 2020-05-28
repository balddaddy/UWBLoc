#ifndef CPROCRAWDATA_H
#define CPROCRAWDATA_H

#include <QObject>
#include <QByteArray>
#include "public.h"
#include "ctaglocalg.h"
#include "ctagtrackalg.h"
#include "ctcpcom.h"

class cProcRawData : public QObject
{
    Q_OBJECT

public:
    cProcRawData();
    ~cProcRawData();
    ERROR_CODE initialize(const int nTagNum, const int nAnchNum, const COORD_XYZ *anchXYZ);

private:
    THREAD_STATUS m_threadStatus;
	
    QByteArray m_tagHead_raw, m_tagHead_loc, m_anchHead;
    TAG_ANCHOR_DATA m_processedData;
    QByteArray m_rawData;
	
    cTagLocAlg *m_tagLocer;
    cTagTrackAlg *m_tagTracker;

    ERROR_CODE (*m_handleDataFun)(TAG_ANCHOR_DATA&, cTCPCom*);
    cTCPCom* m_tcpServer;
	
    QMutex m_mutex_threadStaus;
    QMutex m_mutex_rawData;
	
private:
    DATA_TYPE judgeDataType(const QByteArray &data, int &index);
    ERROR_CODE cutRawData(QByteArray &output, DATA_TYPE &dataType);

    ERROR_CODE parseRawData(const QByteArray &data, const DATA_TYPE &dataType, int &nTagID);

    void setThreadStatus(THREAD_STATUS &status);
    THREAD_STATUS getThreadStatus(void);

public:
    ERROR_CODE initialize(void);
    ERROR_CODE pauseThread(void);
    ERROR_CODE continueThread(void);
    ERROR_CODE stopThread(void);

    static ERROR_CODE addRawData(QByteArray rawData, cProcRawData *thisClass);

    void setHandleDataFun(ERROR_CODE (*handleDataFun)(TAG_ANCHOR_DATA&, cTCPCom*), cTCPCom* tcpPort);
public slots:
    void doWorks(void);
	
signals:
	void workReady(void);
};

#endif // CPROCRAWDATA_H
