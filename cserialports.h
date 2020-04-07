#ifndef CSERIALPORTS_H
#define CSERIALPORTS_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#include <QList>

typedef enum _ERROR_CODE {
    _ERROR_CODE_NOTFIND = 0,
    _ERROR_CODE_OPEN_FAIL,
    _ERROR_CODE_FIND_SUCC,
    _ERROR_CODE_OPEN_SUCC,
    _ERROR_CODE_CONNET_FAIL,
    _ERROR_CODE_CONNET_SUCC,
    _ERROR_CODE_READ_FAIL,
    _ERROR_CODE_READ_SUCC,
    _ERROR_CODE_TIMEOUT
} ERROR_CODE;

class CSerialPorts
{
public:
    CSerialPorts(void);
    ~CSerialPorts(void);

private:
    QList<QSerialPort*> m_deviceports;
    QList<QSerialPortInfo> m_port_info;
    QStringList m_ports_list;

private:
    void findSerialDevices(void); // find all tags and BSs
    ERROR_CODE openSerialDevices(QSerialPort* device);
    void closePort(QSerialPort &device);
    QByteArray readData(QSerialPort* device);
    void writeData(QSerialPort* device, const QByteArray &data);
    void handleError(QSerialPort::SerialPortError error);
public:
    ERROR_CODE openDevices(void);
    int getDeviceNum(void);
    ERROR_CODE testDevices(void);
};

#endif // CSERIALPORTS_H
