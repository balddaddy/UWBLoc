#include <QDebug>

#include "cserialports.h"

CSerialPorts::CSerialPorts(void) : m_isPrintingInfo(false), m_threadStatus(_THREAD_STATUS_STOP)
{
#ifdef _OS_WIN
            m_baudrate = QSerialPort::Baud115200;
#endif
#ifdef _OS_LINUX
            m_baudrate = QSerialPort::Baud9600;
#endif
    m_deviceports.clear();
    m_ports_info.clear();
    m_ports_list.clear();
    m_dataToWrite.clear();
    connect(this, SIGNAL(workReady()), this, SLOT(doWorks()));
}

CSerialPorts::~CSerialPorts(void)
{
    for (int id = 0; id < m_deviceports.size(); id++)
    {
        if (m_deviceports[id]->isOpen())
        {
            m_deviceports[id]->close();
            delete m_deviceports[id];
        }
    }
    m_deviceports.clear();
    m_ports_info.clear();
    m_ports_list.clear();
}

void CSerialPorts::findDevices(void)
{
    m_threadStatus = _THREAD_STATUS_STOP;

    foreach(const QSerialPortInfo &_port, QSerialPortInfo::availablePorts())
    {
        if((_port.description()==DEVICE_NAME_WIN) || (_port.description()==DEVICE_NAME_LNX) || (_port.description()==DEVICE_NAME_MAC))
        {
            int nIndex = m_ports_list.indexOf(_port.portName());
            if (nIndex == -1)
            {
                m_ports_info += _port;
                m_ports_list += _port.portName();
                QSerialPort* tmpDevice = new QSerialPort(nullptr);
                tmpDevice->setPort(_port);
                m_deviceports += tmpDevice;
                if (m_isPrintingInfo){
                    qDebug() << "Find a new device: "
                             << _port.portName() << _port.vendorIdentifier() << _port.productIdentifier()
                             << _port.hasProductIdentifier() << _port.hasVendorIdentifier() << _port.isBusy()
                             << _port.manufacturer() << _port.description()  << endl;
                }
            }
            else
            {
                continue;
            }
        }
    }
    if (m_ports_info.isEmpty())
    {
        qDebug() << "Can't find any devices!" << endl;
    }
    else {
        if (m_isPrintingInfo)
        {
            qDebug() << "All the devices are as following: " << endl;
            foreach (const QSerialPortInfo &_port, m_ports_info)
            {
                qDebug() << _port.portName() << _port.description() << endl;
            }
        }
    }
}

ERROR_CODE CSerialPorts::openDevice(QSerialPort* device)
{
    ERROR_CODE error_code;
    if (!device->isOpen())
    {
        if (device->open(QIODevice::ReadWrite))
        {
            device->setBaudRate(m_baudrate);
            device->setDataBits(QSerialPort::Data8);
            device->setParity(QSerialPort::NoParity);
            device->setStopBits(QSerialPort::OneStop);
            device->setFlowControl(QSerialPort::NoFlowControl);

            qDebug() << "Connected to" << device->portName() << "" << endl;
            error_code = _ERROR_CODE_SUCC;
        }
        else {
            qDebug() << "Open device fails! Error:" << device->error() << endl;
            device->close();
            error_code = _ERROR_CODE_FAIL;
        }
    }
    else {
        qDebug() << "Warning: Device has already been opened!" << endl;
        error_code = _ERROR_CODE_SUCC;
    }
    return error_code;
}

ERROR_CODE CSerialPorts::closeDevice(QSerialPort &device)
{
    ERROR_CODE error_code;
    if (device.isOpen())
    {
        device.close();
        error_code = _ERROR_CODE_SUCC;
        qDebug() << "Device" << device.portName() << "is closed." << endl;
    }
    else
    {
        error_code = _ERROR_CODE_FAIL;
        qDebug() << "Device" << device.portName() << "has already been closed." << endl;
    }
    return error_code;
}

QByteArray CSerialPorts::readData(QSerialPort* device)
{
    QByteArray data;
    data.clear();
    while(device->waitForReadyRead(100)){
        data = device->readAll();
    }
    if (data.isEmpty() && m_isPrintingInfo)
    {
        qDebug() << "Can't receivie any data from Device" << device->portName() << "." << endl;
    }
    return data;
}

void CSerialPorts::writeData(QSerialPort* device, const QByteArray &data)
{
    if (data.isEmpty())
        return;
    if (device->isOpen())
    {
        device->write(data);
        if (m_isPrintingInfo)
            qDebug() << "Sending msg to devices successfully!" << endl;
    }
    else
        if (m_isPrintingInfo)
            qDebug() << "Sending msg to devices failed!" << endl;
}

void CSerialPorts::setThreadStatus(THREAD_STATUS &status)
{
    m_mutex_threadStaus.lock();
    m_threadStatus = status;
    m_mutex_threadStaus.unlock();
}

int CSerialPorts::getDeviceNum(void)
{
    return  m_deviceports.length();
}

void CSerialPorts::switchPrintOnOff(void)
{
    m_mutex_PrintStatus.lock();
    m_isPrintingInfo = !m_isPrintingInfo;
    if (m_isPrintingInfo)
        qDebug() << "Printing information function is on" << endl;
    else
        qDebug() << "Printing information function is off" << endl;
    m_mutex_PrintStatus.unlock();
}

ERROR_CODE CSerialPorts::initialize(void)
{
    ERROR_CODE err_code;
    findDevices();
//    m_threadStatus = _THREAD_STATUS_WORKING;
//    emit workReady();
    if (m_deviceports.isEmpty())
    {
        err_code = _ERROR_CODE_TIMEOUT;
        qDebug() << "Can't initialize devices." << endl;
    }
    else
    {
        for (int id = 0; id < m_deviceports.length(); id++)
        {
            err_code = openDevice(m_deviceports.at(id));
        }
        m_threadStatus = _THREAD_STATUS_WORKING;
        emit workReady();
        qDebug() << "Initialized devices." << endl;
    }
    return err_code;
}

ERROR_CODE CSerialPorts::pauseSerialPort(void)
{
    ERROR_CODE err_code;
    if (m_threadStatus == _THREAD_STATUS_WORKING)
    {
        THREAD_STATUS status = _THREAD_STATUS_PAUSE;
        setThreadStatus(status);
        err_code = _ERROR_CODE_SUCC;
        qDebug() << "Pause Serial Port thread successfully." << endl;
    }
    else
        err_code = _ERROR_CODE_FAIL;
    return err_code;
}

ERROR_CODE CSerialPorts::continueSerialPort(void)
{
    ERROR_CODE err_code;
    if (m_threadStatus == _THREAD_STATUS_PAUSE)
    {
        THREAD_STATUS status = _THREAD_STATUS_WORKING;
        setThreadStatus(status);
        err_code = _ERROR_CODE_SUCC;
        qDebug() << "Continue Serial Port thread successfully." << endl;
    }
    else
        err_code = _ERROR_CODE_FAIL;
    return err_code;
}

ERROR_CODE CSerialPorts::stopSerialPort(void)
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

void CSerialPorts::setDataToSend(QByteArray data)
{
    m_mutex_dataToWrite.lock();
    m_dataToWrite.append(data);
    m_mutex_dataToWrite.unlock();
}

void CSerialPorts::setHandleDataFun(ERROR_CODE (*handleDataFun)(QByteArray))
{
    m_handleDataFun = handleDataFun;
}

void CSerialPorts::doWorks(void)
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
                qDebug() << "\r Serial Port thread has paused." << endl;
            }
        }
        else if(status == _THREAD_STATUS_WORKING)
        {
            for (int id = 0; id < m_deviceports.length(); id++)
            {
                QSerialPort* device = nullptr;
                device = m_deviceports.at(id);
                QByteArray dataToRecv = this->readData(device);
                m_handleDataFun(dataToRecv);
                if (!m_dataToWrite.isEmpty())
                    this->writeData(device, m_dataToWrite);
            }
            if (m_isPrintingInfo)
            {
                qDebug() << "Serial Port thread is working." << endl;
            }
        }
        m_mutex_threadStaus.lock();
        status = m_threadStatus;
        m_mutex_threadStaus.unlock();
    }
    if (m_isPrintingInfo)
    {
        qDebug() << "Serial Port thread quits." << endl;
    }
}
