#include <QDebug>

#include "cserialports.h"


#define DEVICE_NAME_WIN ("STMicroelectronics Virtual COM Port")
#define DEVICE_NAME_LNX ("Virtual COM Port")
#define DEVICE_NAME_MAC ("Virtual COM Port")
#define INST_REPORT_LEN   (65)
#define INST_REPORT_LEN_HEADER (20)
#define INST_VERSION_LEN  (16)
#define INST_CONFIG_LEN   (1)

CSerialPorts::CSerialPorts(void) : m_isReadingData(false), m_isPrintingData(false)
{
#ifdef _OS_WIN
            m_baudrate = QSerialPort::Baud115200;
#endif
#ifdef _OS_LINUX
            m_baudrate = QSerialPort::Baud9600;
#endif
    m_deviceports.clear();  // creat a new serial-port
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
}


void CSerialPorts::findSerialDevices(void)
{
    m_ports_list.clear();
    m_port_info.clear();

    qDebug() << "List all serial-ports as following:" << endl;
    foreach(const QSerialPortInfo &_port, QSerialPortInfo::availablePorts())
    {
        qDebug() << _port.portName() << _port.vendorIdentifier() << _port.productIdentifier()
                 << _port.hasProductIdentifier() << _port.hasVendorIdentifier() << _port.isBusy()
                 << _port.manufacturer() << _port.description()  << endl;

        if((_port.description()==DEVICE_NAME_WIN) || (_port.description()==DEVICE_NAME_LNX) || (_port.description()==DEVICE_NAME_MAC))
        {
            m_port_info += _port;
            m_ports_list += _port.portName();
            QSerialPort* tmpDevice = new QSerialPort(nullptr);
            tmpDevice->setPort(_port);
            m_deviceports += tmpDevice;
        }
    }
    if (m_port_info.isEmpty())
    {
        qDebug() << "Can't find any devices!" << endl;
    }
    else {
        qDebug() << "The anchor's serial ports are as following: " << endl;
        foreach (const QSerialPortInfo &_port, m_port_info)
        {
            qDebug() << _port.portName() << _port.description() << endl;
        }
    }
}

ERROR_CODE CSerialPorts::openSerialDevices(QSerialPort* device)
{
    ERROR_CODE error_code;
    if (!device->isOpen())
    {
        if (device->open(QIODevice::ReadWrite))
        {
#ifdef _OS_LINUX
            device->setBaudRate(QSerialPort::Baud9600);
#endif
#ifdef _OS_WIN
            device->setBaudRate(QSerialPort::Baud115200);
#endif
            device->setDataBits(QSerialPort::Data8);
            device->setParity(QSerialPort::NoParity);
            device->setStopBits(QSerialPort::OneStop);
            device->setFlowControl(QSerialPort::NoFlowControl);

            qDebug() << "Connected to" << device->portName() << "" << endl;
            error_code = _ERROR_CODE_OPEN_SUCC;
        }
        else {
            qDebug() << "Open device fails! Error:" << device->error() << endl;
            device->close();
            error_code = _ERROR_CODE_OPEN_FAIL;
        }
    }
    else {
        qDebug() << "Warning: Device has already been opened!" << endl;
        error_code = _ERROR_CODE_OPEN_SUCC;
    }
    return error_code;
}

QByteArray CSerialPorts::readData(QSerialPort* device)
{
    QByteArray data;
    data.clear();
    int nCount = 0;
    while(device->waitForReadyRead(3000)){
        nCount++;
        if(nCount > 100)
            break;
        data = device->readAll();
        return data;
    }
    return data;
}

void CSerialPorts::writeData(QSerialPort* device, const QByteArray &data)
{
    if (device->isOpen())
    {
        device->write(data);
        qDebug() << "Sending msg to devices successfully!" << endl;
    }
    else
        qDebug() << "Sending msg to devices failed!" << endl;
}

void CSerialPorts::closePort(QSerialPort &device)
{
    device.close();
    qDebug() << "Device" << device.portName() << "is closed" << endl;
}

void CSerialPorts::openDevices(ERROR_CODE& err)
{
    this->findSerialDevices();
    int nCount = m_deviceports.length();
    if (nCount == 0)
    {
        err = _ERROR_CODE_NOTFIND;
    }
    else{
        foreach (QSerialPort* _port, m_deviceports)
        {
            err = this->openSerialDevices(_port);
            if (err == _ERROR_CODE_OPEN_FAIL)
                break;
        }
        err = _ERROR_CODE_FIND_SUCC;
    }
}

int CSerialPorts::getDeviceNum(void)
{
    return  m_deviceports.length();
}

ERROR_CODE CSerialPorts::testDevices(void)
{
    ERROR_CODE err;
    this->openDevices(err);
    int nNum = getDeviceNum();
    if (nNum == 0)
    {
        err = _ERROR_CODE_NOTFIND;
        return  err;
    }
    for (int nid = 0; nid < nNum; nid++)
    {
        for (int niid = 0; niid < 100; niid++)
        {
            QByteArray data = this->readData(m_deviceports[nid]);
            if (data.isEmpty())
                return  _ERROR_CODE_READ_FAIL;
            qDebug() << data << endl;
        }
    }
    err = _ERROR_CODE_READ_SUCC;
    return err;
}

