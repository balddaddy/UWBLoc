#include <QDebug>
#include <QObject>

#include "cserialports.h"


#define DEVICE_STR ("STMicroelectronics Virtual COM Port")
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

    qDebug() << "List all serial-ports bellow: \n";
    foreach(const QSerialPortInfo &_port, QSerialPortInfo::availablePorts())
    {
        qDebug() << _port.portName() << _port.vendorIdentifier() << _port.productIdentifier()
                 << _port.hasProductIdentifier() << _port.hasVendorIdentifier() << _port.isBusy()
                 << _port.manufacturer() << _port.description() << "\n";

        if(_port.description()==DEVICE_STR)
        {
            m_port_info += _port;
            m_ports_list += _port.portName();
        }
    }
    if (m_port_info.isEmpty())
    {
        qDebug() << "Can't find any Tags or Anchors!\n";
    }
    else {
        qDebug() << "The anchor's serial ports are as following: \n";
        m_nDeviceCount = m_ports_list.count();
        m_deviceports = new QSerialPort[m_nDeviceCount];
        int nID = 0;
        foreach (const QSerialPortInfo &_port, m_port_info)
        {
            m_deviceports[nID++].setPort(_port);
            qDebug() << _port.portName() << _port.description() << "\n";
        }
    }
}

int CSerialPorts::openSerialDevices(QSerialPort &device)
{
    int nError = 0;
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
        }
        else {
            qDebug() << "Open device fails! Error:" << device.error() << "\n";
            device.close();
            nError = -1;
        }
    }
    else {
        qDebug() << "Warning: Device has already been opened!\n";
        nError = 1;
    }
    return nError;
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

int CSerialPorts::testDevices(void)
{
    int nError;
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
}
