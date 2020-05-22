#ifndef CPROCRAWDATA_H
#define CPROCRAWDATA_H

#include <QObject>
#include <QByteArray>
#include <QMutex>
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
	bool m_isPrintingInfo;
    THREAD_STATUS m_threadStatus;
	
    QMutex m_mutex_threadStaus;
	QMutex m_mutex_PrintStatus;
    QMutex m_mutex_rawData;
	
    QByteArray m_tagHead_raw, m_tagHead_loc, m_anchHead;
    TAG_ANCHOR_DATA m_processedData;
    QByteArray m_rawData;
	
    cTagLocAlg *m_tagLocer;
    cTagTrackAlg *m_tagTracker;

    ERROR_CODE (*m_handleDataFun)(TAG_ANCHOR_DATA&, cTCPCom*);
    cTCPCom* m_tcpPort;
private:
    void setThreadStatus(THREAD_STATUS &status);
    ERROR_CODE cutRawData(QByteArray& output, DATA_TYPE &dataType);
    DATA_TYPE judgeDataType(QByteArray& data, int &index);

public:
    void switchPrintOnOff(void);

    static ERROR_CODE addRawData(QByteArray rawData, cProcRawData *thisClass);
	
    ERROR_CODE initialize(void);
    ERROR_CODE pauseThread(void);
    ERROR_CODE continueThread(void);
    ERROR_CODE stopThread(void);

    void setHandleDataFun(ERROR_CODE (*handleDataFun)(TAG_ANCHOR_DATA&, cTCPCom*), cTCPCom* tcpPort);
public slots:
    void doWorks(void);
	
signals:
	void workReady(void);
};

#endif // CPROCRAWDATA_H
