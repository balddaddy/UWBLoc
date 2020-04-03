#include <QDebug>
#include <QObject>

#include "cserialports.h"


#define DEVICE_STR_WIN ("STMicroelectronics Virtual COM Port")
#define DEVICE_STR_LINUX ("Virtual COM Port")
#define INST_REPORT_LEN   (65)
#define INST_REPORT_LEN_HEADER (20)
#define INST_VERSION_LEN  (16)
#define INST_CONFIG_LEN   (1)

CSerialPorts::CSerialPorts(void)
{
    m_nDeviceCount = 0;
    m_deviceports = NULL;  // creat a new serial-port
}

CSerialPorts::~CSerialPorts(void)
{
    for (int id = 0; id < m_nDeviceCount; id++)
    {
        if (m_deviceports[id].isOpen()){
            m_deviceports[id].close();
        }
    }
    delete[] m_deviceports;
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

        if((_port.description()==DEVICE_STR_LINUX) || (_port.description()==DEVICE_STR_WIN))
        {
            m_port_info += _port;
            m_ports_list += _port.portName();
        }
    }
    if (m_port_info.isEmpty())
    {
        qDebug() << "Can't find any devices!" << endl;
    }
    else {
        qDebug() << "The anchor's serial ports are as following: \n";
        m_nDeviceCount = m_ports_list.count();
        m_deviceports = new QSerialPort[m_nDeviceCount];
        int nID = 0;
        foreach (const QSerialPortInfo &_port, m_port_info)
        {
            m_deviceports[nID++].setPort(_port);
            qDebug() << _port.portName() << _port.description() << endl;
        }
    }
}

ERROR_CODE CSerialPorts::openSerialDevices(QSerialPort &device)
{
    ERROR_CODE error_code;
    if (!device.isOpen())
    {
        if (device.open(QIODevice::ReadWrite))
        {
            device.setBaudRate(QSerialPort::Baud115200);
            device.setDataBits(QSerialPort::Data8);
            device.setParity(QSerialPort::NoParity);
            device.setStopBits(QSerialPort::OneStop);
            device.setFlowControl(QSerialPort::NoFlowControl);

            qDebug() << "Connected to" << device.portName() << "\n";
            writeData(device, "deca$q");
            qDebug() << "send \"deca$\"\n" ;
            error_code = _ERROR_CODE_OPEN_SUCC;
        }
        else {
            qDebug() << "Open device fails! Error:" << device.error() << "\n";
            device.close();
            error_code = _ERROR_CODE_OPEN_FAIL;
        }
    }
    else {
        qDebug() << "Warning: Device has already been opened!\n";
        error_code = _ERROR_CODE_OPEN_SUCC;
    }
    return error_code;
}

QByteArray CSerialPorts::readData(QSerialPort &device)
{
    QByteArray data; data.clear();
    while(device.waitForReadyRead(3000)){
        QByteArray data = device.readAll();
    }
    if (data.isEmpty())
        qDebug() << "Can't read any data from device" << device.portName() << ".\n";
    else
        qDebug() << "Data: " << data <<".\n";
    return data;
}

void CSerialPorts::writeData(QSerialPort &device, const QByteArray &data)
{
    if (device.isOpen())
    {
        device.write(data);
        qDebug() << "Sending msg to devices successfully!\n";
    }
    else
        qDebug() << "Sending msg to devices failed!\n";
}

void CSerialPorts::closePort(QSerialPort &device)
{
    device.close();
    qDebug() << "Device" << device.portName() << "is closed.\n";
}

ERROR_CODE CSerialPorts::openDevices(void)
{
    ERROR_CODE error_code;
    this->findSerialDevices();
    if (m_nDeviceCount == 0)
    {
        error_code = _ERROR_CODE_NOTFIND;
    }
    else{
        for (int nid = 0; nid < m_nDeviceCount; nid++)
        {
            error_code = this->openSerialDevices(m_deviceports[nid]);
            if (error_code == _ERROR_CODE_OPEN_FAIL)
                return error_code;
        }
        error_code = _ERROR_CODE_FIND_SUCC;
    }
    return error_code;
}


int CSerialPorts::testDevices(void)
{
    int nError = 0;
    this->findSerialDevices();
    for (int nid = 0; nid < m_nDeviceCount; nid++)
    {
        nError = this->openSerialDevices(m_deviceports[nid]);
    }
    for (int nid = 0; nid < m_nDeviceCount; nid++)
    {
        int nCount = 0;
        QByteArray data = this->readData(m_deviceports[nid]);
        if (data.isEmpty())
        {
            qDebug() << "Data of Device:" << m_deviceports[nid].portName() << "is empty!\n";
            continue;
        }
        nCount++;
        if(nCount > 100){
            this->closePort(m_deviceports[nid]);
            break;
        }
    }
    return nError;
}

