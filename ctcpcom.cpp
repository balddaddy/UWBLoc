#include <iostream>
#include "public.h"
#include "ctcpcom.h"

using namespace std;

cTCPCom::cTCPCom()
    :m_isPrintingInfo(false),
      m_threadStatus(_THREAD_STATUS_STOP),
      m_tcpSocket(new QTcpSocket(this))

{
    m_in.setDevice(m_tcpSocket);
    m_in.setVersion(QDataStream::Qt_5_0);
    m_out.setDevice(m_tcpSocket);
    m_out.setVersion(QDataStream::Qt_5_0);
    connect(this, SIGNAL(workReady()), this, SLOT(doWorks()));
}

cTCPCom::~cTCPCom()
{
    m_tcpSocket->abort();
    delete m_tcpSocket;
}

ERROR_CODE cTCPCom::connectToServer(void)
{
    ERROR_CODE err_code;
    m_tcpSocket->connectToHost(m_hostAddr,m_hostPort);
    bool succ = m_tcpSocket->waitForConnected(3000);
    if (succ){
        err_code = _ERROR_CODE_SUCC;
    }
    else
        err_code = _ERROR_CODE_TIMEOUT;
    return err_code;
}

QByteArray cTCPCom::readData(void)
{
    QByteArray buffer; buffer.clear();
    bool succ = m_tcpSocket->waitForReadyRead(300);
    if (succ)
    {
        buffer = m_tcpSocket->readAll();
        if (m_isPrintingInfo)
            cout << "Receive data:\t" << buffer.data() << endl;
    }
    else
    {
        if (m_isPrintingInfo)
            cout << "Receive data timeout." << endl;
    }
    return  buffer;
}

void cTCPCom::writeData(QByteArray buffer)
{
    if (buffer.isEmpty())
        return;
    m_tcpSocket->write(buffer.data(),buffer.size());
    if (m_isPrintingInfo)
        cout << "Send data:\t" << buffer.data() << endl;
}

void cTCPCom::setThreadStatus(THREAD_STATUS &status)
{
    m_mutex_threadStaus.lock();
    m_threadStatus = status;
    m_mutex_threadStaus.unlock();
}

bool cTCPCom::isConnected(void)
{
    bool isConnceted = false;
    isConnceted = m_tcpSocket->waitForConnected(100);
    return isConnceted;
}

void cTCPCom::switchPrintOnOff(void)
{
    m_mutex_PrintStatus.lock();
    m_isPrintingInfo = !m_isPrintingInfo;
    m_mutex_PrintStatus.unlock();
}

ERROR_CODE cTCPCom::initialize(QString serverIP, int serverPort)
{
    ERROR_CODE err_code;
    if (serverPort != 1888)
        err_code = _ERROR_CODE_FAIL;
    m_hostAddr.setAddress(serverIP);
    m_hostPort = serverPort;
    err_code = this->connectToServer();
    if (err_code == _ERROR_CODE_SUCC)
        emit workReady();
    return err_code;
}


ERROR_CODE cTCPCom::pauseConnection(void)
{
    ERROR_CODE err_code;
    if (m_threadStatus == _THREAD_STATUS_WORKING)
    {
        THREAD_STATUS status = _THREAD_STATUS_PAUSE;
        setThreadStatus(status);
        err_code = _ERROR_CODE_SUCC;
        qDebug() << "Paused TCP/IP thread successfully." << endl;
    }
    else
        err_code = _ERROR_CODE_FAIL;
    return err_code;
}

ERROR_CODE cTCPCom::continueConnection(void)
{
    ERROR_CODE err_code;
    if (m_threadStatus == _THREAD_STATUS_PAUSE)
    {
        THREAD_STATUS status = _THREAD_STATUS_WORKING;
        setThreadStatus(status);
        err_code = _ERROR_CODE_SUCC;
        qDebug() << "Continued TCP/IP thread successfully." << endl;
    }
    else
        err_code = _ERROR_CODE_FAIL;
    return err_code;
}

ERROR_CODE cTCPCom::stopConnection(void)
{
    ERROR_CODE err_code;
    if ((m_threadStatus == _THREAD_STATUS_PAUSE) || (m_threadStatus = _THREAD_STATUS_WORKING))
    {
        THREAD_STATUS status = _THREAD_STATUS_STOP;
        setThreadStatus(status);
        err_code = _ERROR_CODE_SUCC;
    }
    else
        err_code = _ERROR_CODE_FAIL;
    return err_code;
}


void cTCPCom::doWorks(void)
{
    m_mutex_threadStaus.lock();
    THREAD_STATUS status = m_threadStatus;
    m_mutex_threadStaus.unlock();
    while (status != _THREAD_STATUS_STOP)
    {
        if (status == _THREAD_STATUS_PAUSE)
        {
            if (m_isPrintingInfo)
            {
                qDebug() << "\r TCP/IP connection thread has paused." << endl;
            }
        }
        else if(status == _THREAD_STATUS_WORKING)
        {
            QByteArray dataToRecv = this->readData();
            m_handleDataFun(dataToRecv);
            if (!m_dataToWrite.isEmpty())
                this->writeData(m_dataToWrite);
            if (m_isPrintingInfo)
            {
                qDebug() << "TCP/IP connection thread is working." << endl;
            }
        }
        m_mutex_threadStaus.lock();
        status = m_threadStatus;
        m_mutex_threadStaus.unlock();
    }
    if (m_isPrintingInfo)
    {
        qDebug() << "TCP/IP connection thread quits." << endl;
    }
}

