#include <QDebug>
#include <QObject>

#include "cserialports.h"


#define DEVICE_STR ("STMicroelectronics Virtual COM Port")
#define DEVICE_STR2 ("USB-SERIAL CH340")
#define DEVICE_STR3 ("Silicon Labs CP210x USB to UART Bridge")
#define INST_REPORT_LEN   (65)
#define INST_REPORT_LEN_HEADER (20)
#define INST_VERSION_LEN  (16)
#define INST_CONFIG_LEN   (1)

CSerialPorts::CSerialPorts(void)
{
    m_deviceports = new QSerialPort(0);  // creat a new serial-port
    m_is_proccing_data = false;
}

CSerialPorts::~CSerialPorts(void)
{
    if (m_deviceports->isOpen())
        m_deviceports->close();
    delete m_deviceports;
}


QStringList CSerialPorts::listPorts()
{
    return  m_ports_list;
}

QSerialPort* CSerialPorts::allPorts()
{
    return m_deviceports;
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

        if((_port.description()==DEVICE_STR) || (_port.description() == DEVICE_STR2))
        {
            m_port_info += _port;
            m_ports_list += _port.portName();
        }
    }
    if (m_port_info.isEmpty())
    {
        qDebug() << "Can't find any Tags or BSs!\n";
    }
    else {
        foreach (const QSerialPortInfo &_port, m_port_info)
        {
            qDebug() << "The BS serial port as following: \n";
            qDebug() << _port.portName() << _port.description() << "\n";
        }
    }
}

int CSerialPorts::openSerialDevices(QSerialPortInfo device)
{
    int nError = 0;
    m_deviceports->setPort(device);
    if (!m_deviceports->isOpen())
    {
        if (m_deviceports->open(QIODevice::ReadWrite))
        {
            m_deviceports->setBaudRate(QSerialPort::Baud115200);
            m_deviceports->setDataBits(QSerialPort::Data8);
            m_deviceports->setParity(QSerialPort::NoParity);
            m_deviceports->setStopBits(QSerialPort::OneStop);
            m_deviceports->setFlowControl(QSerialPort::NoFlowControl);

            qDebug() << "Connected to" << device.portName() << "\n";
            writeData("deca$q");
            qDebug() << "send \"deca$\"\n" ;
        }
        else {
            qDebug() << "Open device fails! Error:" << m_deviceports->error() << "\n";
            m_deviceports->close();
            nError = -1;
        }
    }
    else {
        qDebug() << "Warning: Device has already been opened!\n";
        nError = 1;
    }
    return nError;
}

void CSerialPorts::readData(void)
{
    QByteArray data = m_deviceports->readAll();
    qDebug() << "Data:: " << data ;
}

void CSerialPorts::writeData(const QByteArray &data)
{
    if (m_deviceports->isOpen())
    {
        m_deviceports->write(data);
    }
    else
        qDebug() << "Sending msg to devices failed!\n";
}

void CSerialPorts::closePort(void)
{
    m_deviceports->close();
}

void CSerialPorts::testDevices(void)
{
    this->findSerialDevices();
    foreach (const QSerialPortInfo &_port, m_port_info)
    {
        this->openSerialDevices(_port);
        int nCount = 0;
        while(m_deviceports->waitForReadyRead(3000)){
            this->readData();
            nCount++;
            if(nCount > 100)
                break;
        }
        qDebug() << "\n@ end of data\n";
        this->closePort();
    }
}

