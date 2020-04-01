#ifndef CSERIALPORTS_H
#define CSERIALPORTS_H

#ifdef _OS_WIN
    #include <QtSerialPort/QSerialPort>
    #include <QtSerialPort/QSerialPortInfo>
    #include <QStringList>
    #include <QList>
#endif

#ifdef _OS_LINUX
    #include <stdio.h>
    #include <string.h>
    #include <sys/types.h>
    #include <fcntl.h>
    #include <vector>
#endif
#ifdef _OS_MACOS

#endif

class CSerialPorts
{
public:
    CSerialPorts(void);
    ~CSerialPorts(void);

private:
    int m_nDeviceCount;
    QSerialPort* m_deviceports;
    QList<QSerialPortInfo> m_port_info;
    QStringList m_ports_list;

private:
    void findSerialDevices(void); // find all tags and BSs
    int  openSerialDevices(QSerialPort &device);
    void closePort(QSerialPort &device);
    QByteArray readData(QSerialPort &device);
    void writeData(QSerialPort &device, const QByteArray &data);
    void handleError(QSerialPort::SerialPortError error);
public:
    int testDevices(void);
};

#endif // CSERIALPORTS_H
