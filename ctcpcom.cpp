#include <QDebug>
#include "ctcpcom.h"

cTCPCom::cTCPCom(void):
      m_threadStatus(_THREAD_STATUS_STOP),
      m_tcpSocket(new QTcpSocket(this)),
      m_handleDataFun(nullptr),
      m_procDataDevice(nullptr)

{
    m_dataToWrite.clear();
    connect(this, SIGNAL(workReady()), this, SLOT(doWorks()));
}

cTCPCom::~cTCPCom(void)
{
    THREAD_STATUS status = _THREAD_STATUS_STOP;
    this->setThreadStatus(status);
    while(this->getThreadStatus()!=_THREAD_STATUS_END)
    {
        // wait untill this thread ended.
    }
    m_tcpSocket->abort();
    delete m_tcpSocket;
    m_dataToWrite.clear();
    m_handleDataFun = nullptr;
    m_procDataDevice = nullptr;
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
        if (getPrintStatus())
            qDebug() << "Receive data:\t" << buffer.data() << endl;
    }
    else
    {
        if (getPrintStatus())
            qDebug() << "Receive data timeout." << endl;
    }
    return  buffer;
}

ERROR_CODE cTCPCom::writeData(QByteArray buffer)
{
    ERROR_CODE err;
    if (buffer.isEmpty())
    {
        err = _ERROR_CODE_FAIL;
        return err;
    }
    m_tcpSocket->write(buffer.data(),buffer.size());
    if (getPrintStatus())
        qDebug() << "Send data:\t" << buffer.data() << endl;
    err = _ERROR_CODE_SUCC;
    return err;
}

void cTCPCom::setThreadStatus(THREAD_STATUS &status)
{
    m_mutex_threadStaus.lock();
    m_threadStatus = status;
    m_mutex_threadStaus.unlock();
}

THREAD_STATUS cTCPCom::getThreadStatus(void)
{
    THREAD_STATUS status;
    m_mutex_threadStaus.lock();
    status = m_threadStatus;
    m_mutex_threadStaus.unlock();
    return status;
}

bool cTCPCom::isConnected(void)
{
    bool isConnceted = false;
    isConnceted = m_tcpSocket->waitForConnected(100);
    return isConnceted;
}

ERROR_CODE cTCPCom::initialize(QString serverIP, int serverPort)
{
    ERROR_CODE err_code;
    THREAD_STATUS status = _THREAD_STATUS_STOP;
    this->setThreadStatus(status);
    if (serverPort != _TCP_PORT){
        err_code = _ERROR_CODE_FAIL;
        if (getPrintStatus())
            qDebug() << "Connection port of TCP server is illegal." << endl;
    }
    if (!m_hostAddr.setAddress(serverIP))
        err_code = _ERROR_CODE_FAIL;
    m_hostPort = serverPort;
    err_code = this->connectToServer();
    if (err_code == _ERROR_CODE_SUCC)
    {
        status = _THREAD_STATUS_WORKING;
        this->setThreadStatus(status);
        emit workReady();
        if (getPrintStatus())
            qDebug() << "Emitted thread working signal of TCP server." << endl;
    }
    return err_code;
}


ERROR_CODE cTCPCom::pauseConnection(void)
{
    ERROR_CODE err_code;
    if (m_threadStatus == _THREAD_STATUS_WORKING)
    {
        THREAD_STATUS status = _THREAD_STATUS_PAUSE;
        this->setThreadStatus(status);
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
        this->setThreadStatus(status);
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
        this->setThreadStatus(status);
        err_code = _ERROR_CODE_SUCC;
    }
    else
        err_code = _ERROR_CODE_FAIL;
    return err_code;
}

ERROR_CODE cTCPCom::setDataToSend(TAG_ANCHOR_DATA &data, cTCPCom* thisTcp)
{
    ERROR_CODE err;
    if (!data.isOutputReady)
    {
        err = _ERROR_CODE_FAIL;
        return err;
    }
    QJsonArray jsData2Send;
    for (int id = 0; id < data.nTagNum; id++)
    {
        QJsonObject data2Send;
        COORD_XYZ xyz = data.getTagXYZ(id);
        data2Send.insert(_JSON_SEND2SERVER_ID, data.nTagID[id]);
        data2Send.insert(_JSON_SEND2SERVER_X, xyz.dx);
        data2Send.insert(_JSON_SEND2SERVER_Y, xyz.dy);
        data2Send.insert(_JSON_SEND2SERVER_Z, xyz.dz);
        jsData2Send.insert(id, data2Send);
    }
    QByteArray array2Send = transJson2Bytearray(jsData2Send);
    thisTcp->m_mutex_dataToWrite.lock();
    thisTcp->m_dataToWrite.append(array2Send);
    thisTcp->m_mutex_dataToWrite.unlock();
    err = _ERROR_CODE_SUCC;
    return err;
}

void cTCPCom::setHandleDataFun(ERROR_CODE (*handleDataFun)(QByteArray, cProcCookedData*), cProcCookedData *device)
{
    m_handleDataFun = handleDataFun;
    m_procDataDevice = device;
}

void cTCPCom::doWorks(void)
{
    THREAD_STATUS status = this->getThreadStatus();
    int nPausePrintCount = 0;
    while (status != _THREAD_STATUS_STOP)
    {
        if (status == _THREAD_STATUS_PAUSE)
        {
            if (getPrintStatus() && (nPausePrintCount < 5))
            {
                nPausePrintCount++;
                qDebug() << "\r TCP/IP connection thread paused." << endl;
            }
        }
        else if(status == _THREAD_STATUS_WORKING)
        {
            nPausePrintCount = 0;
            QByteArray dataToRecv = this->readData();
            m_handleDataFun(dataToRecv, m_procDataDevice);
            m_mutex_dataToWrite.lock();
            if (!m_dataToWrite.isEmpty())
            {
                this->writeData(m_dataToWrite);
                m_dataToWrite.clear();
            }
            m_mutex_dataToWrite.unlock();
            if (getPrintStatus())
            {
                qDebug() << "TCP/IP connection thread is working." << endl;
            }
        }
        status = this->getThreadStatus();
    }
    if (getPrintStatus())
    {
        qDebug() << "TCP/IP connection thread quits." << endl;
    }
    status = _THREAD_STATUS_END;
    this->setThreadStatus(status);
}

